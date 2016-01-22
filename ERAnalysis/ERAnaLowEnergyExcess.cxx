#ifndef ERTOOL_ERANALOWENERGYEXCESS_CXX
#define ERTOOL_ERANALOWENERGYEXCESS_CXX

#include "ERAnaLowEnergyExcess.h"

namespace ertool {

	ERAnaLowEnergyExcess::ERAnaLowEnergyExcess(const std::string& name)
		: AnaBase(name)
		, _result_tree(nullptr)
	{

		PrepareTreeVariables();

		// TPC.Min(0 + 10,
		//         -(::larutil::Geometry::GetME()->DetHalfHeight()) + 10,
		//         0 + 10);

		// TPC.Max(2 * (::larutil::Geometry::GetME()->DetHalfWidth()) - 10,
		//         ::larutil::Geometry::GetME()->DetHalfHeight() - 10,
		//         ::larutil::Geometry::GetME()->DetLength() - 10);

		// set default energy cut (for counting) to 0
		_eCut = 0;
		if (_LEESample_mode) {
			_rw.set_debug(false);
			_rw.set_source_filename("/Users/davidkaleko/larlite/UserDev/LowEnergyExcess/LEEReweight/source/LEE_Reweight_plots.root");
			_rw.set_n_generated_events(6637);
			_rw.initialize();
		}

		// Build Box for TPC active volume
		_vactive  = ::geoalgo::AABox(0,
					     -larutil::Geometry::GetME()->DetHalfHeight(),
					     0,
					     2 * larutil::Geometry::GetME()->DetHalfWidth(),
					     larutil::Geometry::GetME()->DetHalfHeight(),
					     larutil::Geometry::GetME()->DetLength());
		
	}


	bool ERAnaLowEnergyExcess::Analyze(const EventData &data, const ParticleGraph &graph)
	{

		_result_tree->SetName(Form("%s", _treename.c_str()));
		
		
		// Reset tree variables
		// Assume we will mis-ID
		ResetTreeVariables();

		_numEvts += 1;

		/// This variable seems to indicate whether a neutrino was reconstructed
		/// (IE a "ccsingleE" was found)
		bool reco = false;

		// size of ParticleSet should be the number of neutrinos found, each associated with a single electron
		auto const& particles = graph.GetParticleArray();

		// First off, if no nue was reconstructed, skip this event entirely.
		for ( auto const & p : particles )
			if ( abs(p.PdgCode()) == 12 ) reco = true;
		if (!reco) return false;

		// Reset the particleID object representing the single electron found
		// singleE_particleID.Reset();
		// Reset the ertool::Shower copy of the ccsingleE-identified ertool::Shower
		singleE_shower.Reset();

		// Loop over particles and find the nue
		for ( auto const & p : particles ) {
			if ( abs(p.PdgCode()) == 12 ) {

				// Save the neutrino vertex to the ana tree
			        _x_vtx = p.Vertex().at(0);
				_y_vtx = p.Vertex().at(1);
				_z_vtx = p.Vertex().at(2);

				// Save the neutrino direction and momentum information to the ana tree
				_nu_theta = p.Momentum().Theta();
				_nu_p = p.Momentum().Length();
				_nu_pt = _nu_p * std::sin(_nu_theta);

				// // Save whether the neutrino verted was inside of fiducial volume
				// if (!(TPC.Contain(p.Vertex())))
				// 	_is_fiducial = false;
				// else _is_fiducial = true;

				// get all descendants of the neutrino in order to calculate total energy deposited
				_e_dep = 0;
				auto const descendants = graph.GetAllDescendantNodes(p.ID());
				_n_children = descendants.size();
				for ( auto const & desc : descendants) {
					auto const & part = graph.GetParticle(desc);
					// does this particle have a reco ID?
					if (part.HasRecoObject() == true) {
						// get the reco object's dep. energy
						// if shower
						if (part.RecoType() == kShower) {
							_e_dep += data.Shower(part.RecoID())._energy;
						}
						if (part.RecoType() == kTrack) {
							_e_dep += data.Track(part.RecoID())._energy;
						}
					}// if the particle has a reco object
				}// for all neutrino descendants

				// Compute the neutrino energy
				_e_nuReco = 0;
				//find the neutrino daughter that is a lepton
				for (auto const& d : p.Children()) {

					auto daught = graph.GetParticle(d);
					// std::cout<<"\tneutrino daughter PDG = "<<daught.PdgCode()<<std::endl;
					// This is the "ccsinglee" electron. Store it's "particleID" to find it in mcparticlegraph later
					if (daught.PdgCode() == 11) {
						// singleE_particleID = ertool_helper::ParticleID(daught.PdgCode(),
						// 	daught.Vertex(),
						// 	daught.Momentum());
						// std::cout<<"Made singleE_particleID with vertex "<<daught.Vertex()<<std::endl;

						singleE_shower = data.Shower(daught.RecoID());
						_e_theta = singleE_shower.Dir().Theta();
						_e_phi = singleE_shower.Dir().Phi();
						// B.I.T.E Analysis
						// Build backward halflines
						::geoalgo::HalfLine ext9(singleE_shower.Start(),singleE_shower.Start()-singleE_shower.Dir());
						::geoalgo::HalfLine ext9_vtx(p.Vertex(),p.Vertex()-p.Momentum().Dir());
						
						//auto crs_tpc_ext0 = _geoalg.Intersection(ext0,_vactive);
						auto crs_tpc_ext9     = _geoalg.Intersection(ext9,_vactive);
						auto crs_tpc_ext9_vtx = _geoalg.Intersection(ext9_vtx,_vactive);
						//double dist0 = _crs_tpc_ext0[0].Dist(singleE_shower.Start());
						double dist9     = crs_tpc_ext9[0].Dist(singleE_shower.Start());
						double dist9_vtx = crs_tpc_ext9_vtx[0].Dist(p.Vertex());
						//if(dist0 > dist9) _dist_2wall = dist9;
						//else _dist_2wall =dist0;
						_dist_2wall =dist9;
						_dist_2wall_vtx =dist9_vtx;
						
						if(crs_tpc_ext9.size() * crs_tpc_ext9_vtx.size()==0)std::cout<<"\n@@@@@@@@@@@@@@@@@@@"<<std::endl;
						
						_is_simple = isInteractionSimple(daught,graph);
						_dedx = data.Shower(daught.RecoID())._dedx;
					}

					_e_nuReco += daught.KineticEnergy();
					if (daught.HasRecoObject() == true) {
						// get the reco object's dep. energy
						if (daught.RecoType() == kTrack) {
							auto mytrack = data.Track(daught.RecoID());
							double current_tracklen = ( mytrack.back() - mytrack.front() ).Length();
							if (current_tracklen > _longestTrackLen) _longestTrackLen = current_tracklen;
						}
					}// if the particle has a reco object
				} // End loop over neutrino children
			}// if we found the neutrino
		}// End loop over particles

		// Get MC particle set
		auto const& mc_graph = MCParticleGraph();
		// Get the MC data
		auto const& mc_data = MCEventData();

		double nu_E_GEV = 1.;
		double e_E_MEV = -1.;
		double e_uz = -2.;

		if (!mc_graph.GetParticleArray().size())
			std::cout << "WARNING: Size of mc particle graph is zero! Perhaps you forgot to include mctruth/mctrack/mcshower?" << std::endl;

		for ( auto const & mc : mc_graph.GetParticleArray() ) {

			// Find the shower particle in the mcparticlegraph that matches the object CCSingleE identified
			// as the single electron (note, the mcparticlegraph object could be a gamma, for example)
			// To do this, grab the ertool Shower in event_data associated with each mcparticlegraph
			// shower particle and compare
			// (note this works for perfect-reco, but there needs more sophisticated methods for reco-reco)
			if (mc.RecoType() == kShower) {
				ertool::Shower mc_ertoolshower = data.Shower(mc);
				// We match ertool showers from mc particle graph to ertool showers from reco particle graphs
				// By comparing the energy to double precision... can consider also comparing _dedx and _time as well
				if (mc_ertoolshower._energy == singleE_shower._energy) {
					auto parent = mc_graph.GetParticle(mc.Parent());
					_parentPDG = parent.PdgCode();
					_mcPDG = mc.PdgCode();
					_mcGeneration = mc.Generation();

					// if (_mcPDG == 11 && _parentPDG == 13) {
					// 	std::cout << "found electron, parent muson. electron at " << singleE_shower.Start() << std::endl;
					// 	if (parent.RecoType() == RecoType_t::kInvisible)
					// 		std::cout << " parent muon is invisible! event ID "
					// 		          << data.Event_ID() << ", run " << data.Run() << ", subrun " << data.SubRun() << std::endl;
					// 	else
					// 		std::cout << "found electron with parent muon. distance between electron and muon end point is "
					// 		          << std::sqrt(
					// 		              singleE_shower.Start().SqDist(mc_data.Track(parent).back())
					// 		          )
					// 		          << std::endl;
					// }
					// if (abs(_parentPDG) == 11) {
					// 	std::cout << "Energy of particle tagged is " << mc_ertoolshower._energy << std::endl;
					// 	std::cout << "PDG of particle tagged is " << _mcPDG << std::endl;
					// 	// std::cout << mc_graph.Diagram() << std::endl;
					// }
				}
			}

			if (!_LEESample_mode) {
				/// This stuff takes the truth neutrino information and fills flux_reweight-relevant
				/// branches in the ttree (used later on to weight events in final stacked histograms)
				if (abs(mc.PdgCode()) == 12 || abs(mc.PdgCode()) == 14 ) {

					int ntype = 0;
					int ptype = 0;
					double E = mc.Energy() / 1e3;

					//	std::cout << E << std::endl;

					if (mc.PdgCode() == 12)       ntype = 1;
					else if (mc.PdgCode() == -12) ntype = 2;
					else if (mc.PdgCode() ==  14) ntype = 3;
					else if (mc.PdgCode() == -14) ntype = 4;

					if (mc.ProcessType() == ::ertool::kK0L) ptype = 3;
					else if (mc.ProcessType() == ::ertool::kKCharged) ptype = 4;
					else if (mc.ProcessType() == ::ertool::kMuDecay) ptype = 1;
					else if (mc.ProcessType() == ::ertool::kPionDecay) ptype = 2;

					if (mc.ProcessType() != ::ertool::kK0L &&
					        mc.ProcessType() != ::ertool::kKCharged &&
					        mc.ProcessType() != ::ertool::kMuDecay &&
					        mc.ProcessType() != ::ertool::kPionDecay) {

						std::cout << " PDG : " << mc.PdgCode() << " Process Type : " << mc.ProcessType() << " from " <<
						          ::ertool::kK0L <<  " or " <<
						          ::ertool::kKCharged << " or " <<
						          ::ertool::kMuDecay << " or " <<
						          ::ertool::kPionDecay << std::endl;
					}

					_weight = _fluxRW.get_weight(E, ntype, ptype);

					break;

				}
			}
			else {
				if (abs(mc.PdgCode()) == 12)
					nu_E_GEV = mc.Energy() / 1000.;
				if (abs(mc.PdgCode()) == 11) {
					e_E_MEV = mc.Energy();
					e_uz = std::cos(mc.Momentum().Theta());
				}
			}
		} // end loop over mc particle graph

		if (_LEESample_mode) {
			if (e_E_MEV < 0 || e_uz < -1 || nu_E_GEV < 0)
				std::cout << "wtf i don't understand" << std::endl;
			_weight = _rw.get_sculpting_weight(e_E_MEV, e_uz) * _rw.get_normalized_weight(nu_E_GEV);
			//temp debug
			_weight *= 2.716;

		}
		_result_tree->Fill();

		return true;
	}

	void ERAnaLowEnergyExcess::ProcessEnd(TFile * fout)
	{

		if (fout) {
			fout->cd();
			_result_tree->Write();
		}

		return;

	}

	void ERAnaLowEnergyExcess::PrepareTreeVariables() {

		if (_result_tree) { delete _result_tree; }

		_result_tree = new TTree(Form("%s", _treename.c_str()), "Result Tree");
		_result_tree->Branch("_numEvts", &_numEvts, "numEvts/I");
		_result_tree->Branch("_is_fiducial", &_is_fiducial, "is_fiducial/O");
		_result_tree->Branch("_e_nuReco", &_e_nuReco, "e_nuReco/D");
		_result_tree->Branch("_e_dep", &_e_dep, "e_dep/D");
		_result_tree->Branch("_weight", &_weight, "weight/D");
		_result_tree->Branch("_parentPDG", &_parentPDG, "parent_PDG/I");
		_result_tree->Branch("_mcPDG", &_mcPDG, "mc_PDG/I");
		_result_tree->Branch("_mcGeneration", &_mcGeneration, "mc_Generation/I");
		_result_tree->Branch("_longestTrackLen", &_longestTrackLen, "longest_tracklen/D");
		_result_tree->Branch("_x_vtx", &_x_vtx, "x_vtx/D");
		_result_tree->Branch("_y_vtx", &_y_vtx, "y_vtx/D");
		_result_tree->Branch("_z_vtx", &_z_vtx, "z_vtx/D");
		_result_tree->Branch("_e_theta", &_e_theta, "_e_theta/D");
		_result_tree->Branch("_e_phi", &_e_phi, "_e_phi/D");
		_result_tree->Branch("_nu_theta", &_nu_theta, "_nu_theta/D");
		_result_tree->Branch("_nu_pt", &_nu_pt, "_nu_pt/D");
		_result_tree->Branch("_nu_p", &_nu_p, "_nu_p/D");
		_result_tree->Branch("_n_children", &_n_children, "_n_children/I");
		_result_tree->Branch("_is_simple", &_is_simple, "_is_simple/O");
		_result_tree->Branch("_dedx", &_dedx, "dedx/D");
		_result_tree->Branch("_dist_2wall", &_dist_2wall, "dist_2wall/D");
		_result_tree->Branch("_dist_2wall_vtx", &_dist_2wall_vtx, "dist_2wall_vtx/D");
		
		return;
	}

	void ERAnaLowEnergyExcess::ResetTreeVariables() {

		_numEvts = 0;
		_is_fiducial = false;
		_e_nuReco = 0;
		_e_dep = 0;
		_parentPDG = -99999;
		_mcPDG = -99999;
		_mcGeneration = -99999;
		_longestTrackLen = 0;
		_x_vtx = -999.;
		_y_vtx = -999.;
		_z_vtx = -999.;
		_e_theta = -999.;
		_e_phi = -999.;
		_nu_p = -999.;
		_nu_pt = -999.;
		_nu_theta = -999.;
		_n_children = -999;
		_is_simple = false;
		_dedx = -999.;
		_dist_2wall_vtx =-999.;
		_dist_2wall = -999.;
		
		return;

	}

	double ERAnaLowEnergyExcess::EnuCaloMissingPt(const std::vector< ::ertool::NodeID_t >& Children, const ParticleGraph &graph) {

		double Enu = 0;          //MeV
		double Elep = 0;         //MeV
		double Ehad = 0;         //MeV
		double pT = 0;           //MeV
		double Es = 30.5;        //MeV
		double mAr = 37211.3261; //MeV
		double mp =  938.28;     //MeV
		double mn = 939.57;      //MeV
		double Emdefect = 8.5;   //MeV //why 8.5? Because en.wikipedia.org/wiki/Nuclear_binding_energy, find something better.
		int nP = 0, nN = 0;
		auto XY = ::geoalgo::Vector(1, 1, 0);


		for (auto const& d : Children) {

			auto daught = graph.GetParticle(d);

			if (daught.PdgCode() == 11 || daught.PdgCode()) {
				Elep += daught.KineticEnergy();
			}
			else if (daught.RecoType() == kTrack || daught.RecoType() == kShower) {
				Ehad += daught.KineticEnergy();
			}

			pT += daught.Momentum().Dot(XY);

			if (daught.PdgCode() == 2212) { nP++;}
			if (daught.PdgCode() == 2112) { nN++;}
		}

		mAr -= nP * mp + nN * mn + (nN + nP) * Emdefect;

		// Enu = Elep + Ehad + Es +
		//       sqrt(pow(pT, 2) + pow(mAr, 2)) - mAr;
		Enu = Elep + Ehad + Es +
		      pow(pT, 2) / (2 * mAr);

		return Enu;
	}

	bool ERAnaLowEnergyExcess::isInteractionSimple(const Particle &singleE, const ParticleGraph &ps) {

		auto const &kids = ps.GetAllDescendantNodes(singleE.ID());
		auto const &bros = ps.GetSiblingNodes(singleE.ID());

		// Number of particles associated with this electron that are not protons, or the single e itself
		size_t _n_else = 0;
		for ( auto const& kid : kids )
			if (ps.GetParticle(kid).PdgCode() != 2212) _n_else++;
		for ( auto const& bro : bros )
			if (ps.GetParticle(bro).PdgCode() != 2212) _n_else++;

		return _n_else ? false : true;

	}

}

#endif
