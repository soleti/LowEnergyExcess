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

	}

	void ERAnaLowEnergyExcess::ProcessBegin() {

		/// Initialize the LEE reweighting package, if in LEE sample mode...
		if (_LEESample_mode) {
			_rw.set_debug(false);
			_rw.set_source_filename("$LARLITE_USERDEVDIR/LowEnergyExcess/LEEReweight/source/LEE_Reweight_plots.root");
			_rw.set_n_generated_events(6637);
			/// The input neutrinos were generated only in the TPC, not the entire cryostat
			_rw.set_events_generated_only_in_TPC(true);
			_rw.initialize();
		}

		// Build Box for TPC active volume
        _vactive  = ::geoalgo::AABox(0, -larutil::Geometry::GetME()->DetHalfHeight(), 0, 2 * larutil::Geometry::GetME()->DetHalfWidth(), larutil::Geometry::GetME()->DetHalfHeight(), larutil::Geometry::GetME()->DetLength());
        _tagger_top_1.Min(-131.6, 571.7, -87.3);
        _tagger_top_1.Max(387.6, 572.7, 1124);

        _tagger_top_2.Min(-304.6, 571.7, 604.8);
        _tagger_top_2.Max(-131.6, 572.7, 950.9);
        
        _tagger_ft.Min(-129.7, -221.5, -84.9);
        _tagger_ft.Max(-131.6, 124.6, 1126.4);
        _tagger_pipe_1.Min(378.3, -247.1, -19.1);
        _tagger_pipe_1.Max(385.7, 252.9, 1192.2);
        _tagger_pipe_2.Min(378.3, -247.1, -192.2);
        _tagger_pipe_2.Max(385.7, 99, -19.1);
        
        _tagger_under.Min(-131.6, -253.4, 210.4);
        _tagger_under.Max(387.6, -254.4, 556.5);

	}
    
    bool ERAnaLowEnergyExcess::OBIntersect(const TVector3& start, const TVector3& end) {

        TVector3 dir = end - start;
        double mag = dir.Mag();
        dir[0] /= mag;
        dir[1] /= mag;
        dir[2] /= mag;
        //if(dir[1]>0) return false;
        //std::cout<<"start "<<start[0]<<" "<<start[1]<<" "<<start[2]<<std::endl;
        //std::cout<<"end   "<<end[0]<<" "<<end[1]<<" "<<end[2]<<std::endl;
        //std::cout<<"dir   "<<dir[0]<<" "<<dir[1]<<" "<<dir[2]<<std::endl;


        double ob_x = start[0] + dir[0] * 1478.28;
        double ob_z = start[2] + dir[2] * 1478.28;

        double radius = sqrt(pow(ob_x-128.175,2)+pow(ob_z-518.5,2));
        bool ob = radius < 741.68;
        return ob;

    }
    
    
    bool ERAnaLowEnergyExcess::IntersectDumbPhaseA(const TVector3& start, const TVector3& end) {

        TVector3 dir = end - start;
        double mag = dir.Mag();
        dir[0] /= mag;
        dir[1] /= mag;
        dir[2] /= mag;

        
        auto const& under_min = _tagger_under.Min();
        auto const& under_max = _tagger_under.Max();
        
        auto const& ft_min = _tagger_ft.Min();
        auto const& ft_max = _tagger_ft.Max();
        
        auto const& pipe_1_min = _tagger_pipe_1.Min();
        auto const& pipe_1_max = _tagger_pipe_1.Max();
        
        auto const& pipe_2_min = _tagger_pipe_2.Min();
        auto const& pipe_2_max = _tagger_pipe_2.Max();


        double under_x = start[0] + dir[0] * under_min[1];
        double under_z = start[2] + dir[2] * under_min[1];

        double ft_y = start[1] + dir[1] * ft_min[0];
        double ft_z = start[2] + dir[2] * ft_min[0];

        double pipe_1_y = start[1] + dir[1] * pipe_1_min[0];
        double pipe_1_z = start[2] + dir[2] * pipe_1_min[0];

        double pipe_2_y = start[1] + dir[1] * pipe_2_min[0];
        double pipe_2_z = start[2] + dir[2] * pipe_2_min[0];

        bool under_hit = ( under_x > under_min[0] && under_x < under_max[0] &&
            under_z > under_min[2] && under_z < under_max[2] );
        
        bool ft_hit = ( ft_y > ft_min[1] && ft_y < ft_max[1] &&
            ft_z > ft_min[2] && ft_z < ft_max[2] );
        
        bool pipe_1_hit = ( pipe_1_y > pipe_1_min[1] && pipe_1_y < pipe_1_max[1] &&
            pipe_1_z > pipe_1_min[2] && pipe_1_z < pipe_1_max[2] );
        
        bool pipe_2_hit = ( pipe_2_y > pipe_2_min[1] && pipe_2_y < pipe_2_max[1] &&
            pipe_2_z > pipe_2_min[2] && pipe_2_z < pipe_2_max[2] );

        bool result = pipe_1_hit || pipe_2_hit || under_hit || ft_hit;
                
        return result;

    }
    
    bool ERAnaLowEnergyExcess::IntersectDumb(const TVector3& start, const TVector3& end) {

        TVector3 dir = end - start;
        double mag = dir.Mag();
        dir[0] /= mag;
        dir[1] /= mag;
        dir[2] /= mag;
        //if(dir[1]>0) return false;
        //std::cout<<"start "<<start[0]<<" "<<start[1]<<" "<<start[2]<<std::endl;
        //std::cout<<"end   "<<end[0]<<" "<<end[1]<<" "<<end[2]<<std::endl;
        //std::cout<<"dir   "<<dir[0]<<" "<<dir[1]<<" "<<dir[2]<<std::endl;

        auto const& top_1_min = _tagger_top_1.Min();
        auto const& top_1_max = _tagger_top_1.Max();

        auto const& top_2_min = _tagger_top_2.Min();
        auto const& top_2_max = _tagger_top_2.Max();
        
        
        double top_1_x = start[0] + dir[0] * top_1_min[1];
        double top_1_z = start[2] + dir[2] * top_1_min[1];

        double top_2_x = start[0] + dir[0] * top_2_min[1];
        double top_2_z = start[2] + dir[2] * top_2_min[1];



        //std::cout<<"upper xs: "<<upper_x<<" "<<upper_z<<std::endl;

        bool top_1_hit = ( top_1_x > top_1_min[0] && top_1_x < top_1_max[0] &&
            top_1_z > top_1_min[2] && top_1_z < top_1_max[2] );

        bool top_2_hit = ( top_2_x > top_2_min[0] && top_2_x < top_2_max[0] &&
            top_2_z > top_2_min[2] && top_2_z < top_2_max[2] );
 
        bool phaseA = IntersectDumbPhaseA(start,end);
 
        bool result = top_1_hit || top_2_hit || phaseA;
        
        //std::cout << result << std::endl << std::endl;
        
        return result;

    }


    bool ERAnaLowEnergyExcess::Intersect(const ::geoalgo::HalfLine& cosmic_dir) {

        ::geoalgo::GeoAlgo alg;
        //auto const i_tagger_ft = alg.Intersection(_tagger_ft,cosmic_dir);

        auto const i_tagger_top_1 = alg.Intersection(_tagger_top_1,cosmic_dir);
        /*auto const i_tagger_top_2 = alg.Intersection(_tagger_top_2,cosmic_dir);
        auto const i_tagger_pipe_1 = alg.Intersection(_tagger_pipe_1,cosmic_dir);
        auto const i_tagger_pipe_2 = alg.Intersection(_tagger_pipe_2,cosmic_dir);
        auto const i_tagger_under = alg.Intersection(_tagger_under,cosmic_dir);
        
        bool top = !(i_tagger_top_1.empty()) || !(i_tagger_top_2.empty());
        bool ft = !(i_tagger_ft.empty());
        bool pipet = !(i_tagger_pipe_1.empty()) || !(i_tagger_pipe_2.empty());
        bool under = !(i_tagger_under.empty());
        */
        return !i_tagger_top_1.empty();
        
    }

	bool ERAnaLowEnergyExcess::Analyze(const EventData &data, const ParticleGraph &graph)
	{

		_result_tree->SetName(Form("%s", _treename.c_str()));

        ::geoalgo::HalfLine cosmic_dir;

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
		if (!reco) {
			// std::cout<<"No reconstructed neutrino in this event."<<std::endl;
			return false;
		}

		// Reset the particleID object representing the single electron found
		// singleE_particleID.Reset();
		// Reset the ertool::Shower copy of the ccsingleE-identified ertool::Shower
		singleE_shower.Reset();

		// Loop over particles and find the nue
		// std::cout << __PRETTY_FUNCTION__ << " loop over particles... " << std::endl;
		for ( auto const & p : particles ) {
			if ( abs(p.PdgCode()) == 12 ) {
				// std::cout << "Found a nue" << std::endl;
				if(p.ProcessType()==kPiZeroMID) _maybe_pi0_MID = true;
				// Get the event timing from the most ancestor particle
				try {
					// Careful: p.Ancestor() returns a NODEID, but the data.Flash() function wants either a flash ID
					// or an actual particle. Instead, use data.Flash(graph.GetParticle(p.Ancestor()))
					_flash_time = data.Flash(graph.GetParticle(p.Ancestor()))._t;
					_summed_flash_PE = data.Flash(graph.GetParticle(p.Ancestor())).TotalPE();
				}
				catch ( ERException &e ) {}// std::cout << " No flash found for ancestor :( " << std::endl;}

				// Save the neutrino vertex to the ana tree
				_x_vtx = p.Vertex().at(0);
				_y_vtx = p.Vertex().at(1);
				_z_vtx = p.Vertex().at(2);
				geoalgo::Vector pos_vtx(_x_vtx , _y_vtx, _z_vtx);
				//if(_vactive.Contain(pos_vtx)>0) std::cout<<"vtx found inside TPC active"<<std::endl;
				//if(_vactive.Contain(pos_vtx)==0) std::cout<<"vtx found outside TPC active"<<std::endl;
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
				// std::cout << "nue's descendants are: ";
				for ( auto const & desc : descendants) {
					auto const & part = graph.GetParticle(desc);
					// std::cout << part.PdgCode() << ", ";
					if(part.PdgCode() == 22) std::cout<<"WTF gamma is daughter of neutrino?"<<std::endl;
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
				// std::cout << std::endl;

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
						//std::cout<<"Made singleE_particleID with vertex "<<daught.Vertex().at(1)<<std::endl;

						singleE_shower = data.Shower(daught.RecoID());
						// std::cout << "singleE_shower actual time is " << singleE_shower._time << std::endl;
						_e_theta = singleE_shower.Dir().Theta();
						_e_phi = singleE_shower.Dir().Phi();
						_e_Edep = singleE_shower._energy;
						_e_CCQE = _eccqecalc.ComputeECCQE(singleE_shower) * 1000.;
						_is_simple = isInteractionSimple(daught, graph);

						///###### B.I.T.E Analysis Start #####
						// Build backward halflines
						//::geoalgo::HalfLine ext9(singleE_shower.Start(), singleE_shower.Start() - singleE_shower.Dir());
						//::geoalgo::HalfLine ext9_vtx(p.Vertex(), p.Vertex() - p.Momentum().Dir());
	
						::geoalgo::Vector inverse_shr_dir(-singleE_shower.Dir()[0],
									  -singleE_shower.Dir()[1],
									  -singleE_shower.Dir()[2]);
						::geoalgo::Vector inverse_vtx_dir(-p.Momentum().Dir()[0],
									  -p.Momentum().Dir()[1],
									  -p.Momentum().Dir()[2]);
						::geoalgo::HalfLine ext9(singleE_shower.Start(),inverse_shr_dir);
						::geoalgo::HalfLine ext9_vtx(p.Vertex(), inverse_vtx_dir);

						//auto crs_tpc_ext0 = _geoalg.Intersection(ext0,_vactive);

						auto crs_tpc_ext9     = _geoalg.Intersection(ext9, _vactive);
						auto crs_tpc_ext9_vtx = _geoalg.Intersection(ext9_vtx, _vactive);
						//double dist0 = _crs_tpc_ext0[0].Dist(singleE_shower.Start());

						double dist9  = 999.;
						double dist9_vtx = 999.;
						if (crs_tpc_ext9.size())     dist9     = crs_tpc_ext9[0].Dist(singleE_shower.Start());
						if (crs_tpc_ext9_vtx.size()) dist9_vtx = crs_tpc_ext9_vtx[0].Dist(p.Vertex());
						//if(dist0 > dist9) _dist_2wall = dist9;
						//else _dist_2wall =dist0;

						_dist_2wall = dist9;
						_dist_2wall_vtx = dist9_vtx;

						// if(!crs_tpc_ext9.size() || !crs_tpc_ext9_vtx.size())std::cout<<"\nHi, I'm a cosmic and I don't intersect TPC."<<std::endl;

						///###### B.I.T.E Analysis END #####
						_is_simple = isInteractionSimple(daught, graph);
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
                    auto ancestor = mc_graph.GetParticle(mc.Ancestor());
                    _ancestorPDG = ancestor.PdgCode();
                    _ancestor_e = ancestor.Energy();
					_parentPDG = parent.PdgCode();
					_mcPDG = mc.PdgCode();
					_mcGeneration = mc.Generation();
                    _parent_e = parent.Energy();
                    TVector3 start, end;
                    
                    if (parent.RecoType() != kTrack) {
                        start.SetXYZ(mc_ertoolshower.Start()[0],mc_ertoolshower.Start()[1],mc_ertoolshower.Start()[2]);                 
                        end.SetXYZ(mc_ertoolshower.Start()[0]+parent.Momentum().at(0), mc_ertoolshower.Start()[1]+parent.Momentum().at(1), mc_ertoolshower.Start()[2]+parent.Momentum().at(2));
                        if (abs(_parentPDG) == 13 || abs(_parentPDG) == 11 || abs(_parentPDG) == 111) {
                            IntersectDumb(start, end) ? _in_tagger = true : _in_tagger = false;
                            IntersectDumbPhaseA(start,end) ? _in_tagger_phaseA = true : _in_tagger_phaseA = false;      
                        }
                        if (parent.Vertex().at(1) > 1478.28) 
                            OBIntersect(start, end) ? _in_ob = true : _in_ob = false;
                        
                    }

                    if(parent.RecoType() == kTrack) {
                        auto trk = mc_data.Track(parent.RecoID());

                        
                        //cosmic_dir.Start(trk[0]);
                        //cosmic_dir.Dir(trk[0][0]-trk[trk.size()-1][0], trk[0][1]-trk[trk.size()-1][1], trk[0][2]-trk[trk.size()-1][2]);
                        
                        start.SetXYZ(trk[0][0], trk[0][1], trk[0][2]);
                        end.SetXYZ(trk[trk.size()-1][0], trk[trk.size()-1][1], trk[trk.size()-1][2]);
                        
                        if (abs(_parentPDG) == 13 || abs(_parentPDG) == 11 || abs(_parentPDG) == 111) {
                            IntersectDumb(start,end) ? _in_tagger = true : _in_tagger = false;      
                            IntersectDumbPhaseA(start,end) ? _in_tagger_phaseA = true : _in_tagger_phaseA = false;      
                        
                        }
                        if (parent.Vertex().at(1) > 1478.28) 
                            OBIntersect(start, end) ? _in_ob = true : _in_ob = false;

                        //Intersect(cosmic_dir) ? std::cout << "Smart yes" << std::endl << std::endl : std::cout << "Smart no" << std::endl << std::endl;
                        //auto top = _geoalg.Intersection(trk_line, _tagger_ft);
                        //std::cout << top.size() << std::endl;
                        
                            
                    }
  
                    _start_x = start.x();
                    _start_y = start.y();
                    _start_z = start.z();
                    _end_x = end.x();
                    _end_y = end.y();
                    _end_z = end.z();
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
				/// branches in the ttree (used later on to weight events in  stacked histograms)
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
		_result_tree->Branch("_parent_e", &_parent_e, "parent_e/D");
		_result_tree->Branch("_ancestorPDG", &_ancestorPDG, "ancestor_PDG/I");
		_result_tree->Branch("_ancestor_e", &_ancestor_e, "ancestor_e/D");
        
		_result_tree->Branch("_mcPDG", &_mcPDG, "mc_PDG/I");
		_result_tree->Branch("_mcGeneration", &_mcGeneration, "mc_Generation/I");
		_result_tree->Branch("_longestTrackLen", &_longestTrackLen, "longest_tracklen/D");
		_result_tree->Branch("_x_vtx", &_x_vtx, "x_vtx/D");
		_result_tree->Branch("_y_vtx", &_y_vtx, "y_vtx/D");
		_result_tree->Branch("_z_vtx", &_z_vtx, "z_vtx/D");
		_result_tree->Branch("_e_theta", &_e_theta, "_e_theta/D");
		_result_tree->Branch("_e_phi", &_e_phi, "_e_phi/D");
		_result_tree->Branch("_e_Edep", &_e_Edep, "_e_Edep/D");
		_result_tree->Branch("_e_CCQE", &_e_CCQE, "_e_CCQE/D");
		_result_tree->Branch("_nu_theta", &_nu_theta, "_nu_theta/D");
		_result_tree->Branch("_nu_pt", &_nu_pt, "_nu_pt/D");
		_result_tree->Branch("_nu_p", &_nu_p, "_nu_p/D");
		_result_tree->Branch("_n_children", &_n_children, "_n_children/I");
		_result_tree->Branch("_is_simple", &_is_simple, "_is_simple/O");
		_result_tree->Branch("_dedx", &_dedx, "dedx/D");
		_result_tree->Branch("_flash_time", &_flash_time, "flash_time/D");
		_result_tree->Branch("_summed_flash_PE", &_summed_flash_PE, "summed_flash_PE/D");
		_result_tree->Branch("_start_x", &_start_x, "start_x/D");
		_result_tree->Branch("_start_y", &_start_y, "start_y/D");
		_result_tree->Branch("_start_z", &_start_z, "start_z/D");
		_result_tree->Branch("_end_x", &_end_x, "end_x/D");
		_result_tree->Branch("_end_y", &_end_y, "end_y/D");
		_result_tree->Branch("_end_z", &_end_z, "end_z/D");
		_result_tree->Branch("_dist_2wall", &_dist_2wall, "dist_2wall/D");
		_result_tree->Branch("_dist_2wall_vtx", &_dist_2wall_vtx, "dist_2wall_vtx/D");
		_result_tree->Branch("_maybe_pi0_MID", &_maybe_pi0_MID, "_maybe_pi0_MID/O");
		_result_tree->Branch("_in_tagger", &_in_tagger, "_in_tagger/O");
		_result_tree->Branch("_in_tagger_phaseA", &_in_tagger_phaseA, "_in_tagger_phaseA/O");
        
		_result_tree->Branch("_in_ob", &_in_ob, "_in_ob/O");

		return;
	}

	void ERAnaLowEnergyExcess::ResetTreeVariables() {

		_numEvts = 0;
		_is_fiducial = false;
		_e_nuReco = 0;
		_e_dep = 0;
		_parentPDG = -99999;
		_parent_e = -999;
        
		_mcPDG = -99999;
		_mcGeneration = -99999;
		_longestTrackLen = 0.;
		_x_vtx = -999.;
		_y_vtx = -999.;
		_z_vtx = -999.;
		_start_x = -999.;
		_start_y = -999.;
		_start_z = -999.;
		_end_x = -999.;
		_end_y = -999.;
		_end_z = -999.;
		_e_theta = -999.;
		_e_phi = -999.;
		_e_Edep = -999.;
		_e_CCQE = -999.;
		_nu_p = -999.;
		_nu_pt = -999.;
		_nu_theta = -999.;
		_n_children = -999;
		_is_simple = false;
		_dedx = -999.;
		_flash_time = -999999999.;
		_summed_flash_PE = -999999999.;
		_dist_2wall_vtx = -999.;
		_dist_2wall = -999.;
		_maybe_pi0_MID = false;
		_in_tagger = false;
		_in_ob = false;

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
