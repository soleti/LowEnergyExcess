#ifndef ERTOOL_ERANANCPI0DEBUG_CXX
#define ERTOOL_ERANANCPI0DEBUG_CXX

#include "ERAnaNCPi0Debug.h"

namespace ertool {

  ERAnaNCPi0Debug::ERAnaNCPi0Debug(const std::string& name) : AnaBase(name), _tree(nullptr)
  {

    if (_tree) { delete _tree; }

    _tree = new TTree("tree", "NCPi0 Debug Tree");
    _tree->Branch("_parentPDG", &_parentPDG, "parent_PDG/I");
    _tree->Branch("_mcPDG", &_mcPDG, "mc_PDG/I");
    _tree->Branch("_e_Edep", &_e_Edep, "_e_Edep/D");
    _tree->Branch("_dedx", &_dedx, "dedx/D");
    _tree->Branch("_n_ertool_showers", &_n_ertool_showers, "_n_ertool_showers/I");
    _tree->Branch("_dist_to_closest_track_start", &_dist_to_closest_track_start, "dist_to_closest_track_start/D");

    return;
  }

  void ERAnaNCPi0Debug::Reset()
  {}

  void ERAnaNCPi0Debug::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaNCPi0Debug::ProcessBegin()
  {}

  bool ERAnaNCPi0Debug::Analyze(const EventData &data, const ParticleGraph &graph)
  {
    /// Clear TTree variables
    _parentPDG = -999;
    _mcPDG = -999;
    _e_Edep = -999.;
    _dedx = -999.;
    _n_ertool_showers = -999;

    auto const& particles = graph.GetParticleArray();

    /// If no nue was reconstructed, skip
    bool reco = false;
    for ( auto const & p : particles )
      if ( abs(p.PdgCode()) == 12 ) reco = true;
    if (!reco) return false;

    /// Store # of ertool showers in the entire event
    _n_ertool_showers = graph.GetParticleNodes(RecoType_t::kShower).size();

    singleE_shower.Reset();
    FlashID_t singleE_flashID = 99999;
    for ( auto const & p : particles ) {

      if ( abs(p.PdgCode()) != 12 ) continue;

      //now p is a nue
      //find the neutrino daughter that is tagged as an electron
      for (auto const& d : p.Children()) {
        auto daught = graph.GetParticle(d);
        if (daught.PdgCode() == 11) {
          singleE_shower = data.Shower(daught.RecoID());
          _e_Edep = singleE_shower._energy;
          _dedx = singleE_shower._dedx;
        }// end having found the singleE shower

        /// get the flash ID associated with the singleE
        try {
          singleE_flashID = data.Flash(graph.GetParticle(p.Ancestor())).FlashID();
        }
        catch ( ERException &e ) {}// std::cout << " No flash found for ancestor :( " << std::endl;}
      }//end looping over neutrino daughters
    }//end loop over reco particle graph

    bool event_of_interest = false;
    auto const& mc_graph = MCParticleGraph();

    for ( auto const & mc : mc_graph.GetParticleArray() ) {
      if (mc.RecoType() == kShower) {
        ertool::Shower mc_ertoolshower = data.Shower(mc);
        if (mc_ertoolshower._energy != singleE_shower._energy) continue;
        _mcPDG = mc.PdgCode();
        auto parent = mc_graph.GetParticle(mc.Parent());
        _parentPDG = parent.PdgCode();
        if (_parentPDG == 111 && _e_Edep > 50.) {
          std::cout << "NC DEBUG! This is a pi0 MID that appears in the stacked histogram. "
                    << "Parent is 111, singleE dep energy is " << _e_Edep << std::endl;
          event_of_interest = true;

          // std::cout << "Here's the MC Particlegraph diagram" << std::endl;
          // std::cout << mc_graph.Diagram() << std::endl;
          // std::cout << "Here's the Reco Particlegraph diagram" << std::endl;
          // std::cout << graph.Diagram() << std::endl;

        }//end if parent pdg is 111 and electron deposits > 50 MEV
      }//end if recotype == shower
    }//end loop over mcparticlegraph

    if (!event_of_interest) return false;

    //Get everything associated with the same flash as the shower, and
    //add up hadronic energy (considering protons and pions for now)
    double Wtotal = 0.;
    if (singleE_flashID != 99999) {
      for ( auto const & t : data.Track() ) {
        FlashID_t i_flashID = 99998;
        auto p = graph.GetParticle(graph.NodeID(t));
        try {i_flashID = data.Flash(graph.GetParticle(p.Ancestor())).FlashID();}
        catch ( ERException &e ) {}
        if (i_flashID == singleE_flashID) {
          std::cout<<"Found something that shares the flash! PDG = "<<p.PdgCode()<<std::endl;
          //found something that shares the flash
          // add up hadronic energy (from protons, pions)
          if (p.PdgCode() == 2212 || p.PdgCode() == 211 ) {
            Wtotal += t._energy;
          }
        } // End if you found something that shares the flash

      } // end loop over particles to add up the hadronic energy
    }//end if singleE_flashID != 99999

    std::cout << " and Wtotal is " << Wtotal << std::endl;
    ///Compute distance to closest track start
    _dist_to_closest_track_start = 999999.;
    for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)) {
      auto const& thatTrack = data.Track(graph.GetParticle(t).RecoID());
      if (thatTrack.Length() < 0.3)
        continue;
      geoalgo::Point_t vtx(3);
      // compare the two tracks
      double IP =  _findRel.FindClosestApproach(singleE_shower, thatTrack, vtx);
      double mydist = vtx.Dist(singleE_shower.Start());
      if (mydist < _dist_to_closest_track_start) _dist_to_closest_track_start = mydist;

    }//end loop over computing distance to closets track start



    _tree->Fill();

    return true;
  }

  void ERAnaNCPi0Debug::ProcessEnd(TFile * fout)
  {
    if (fout) {
      fout->cd();
      _tree->Write();
    }

    return;
  }

}

#endif
