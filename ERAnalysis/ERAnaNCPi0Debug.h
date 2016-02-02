/**
 * \file ERAnaNCPi0Debug.h
 *
 * \ingroup ERAnalysis
 *
 * \brief Class specifically to identify singleE MIDs on NCpi0 events
 *
 * @author davidkaleko
 */

/** \addtogroup ERAnalysis

    @{*/

#ifndef ERTOOL_ERANANCPI0DEBUG_H
#define ERTOOL_ERANANCPI0DEBUG_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"

namespace ertool {

  /**
     \class ERAnaNCPi0Debug
     User custom Analysis class made by kazuhiro
   */
  class ERAnaNCPi0Debug : public AnaBase {

  public:

    /// Default constructor
    ERAnaNCPi0Debug(const std::string& name = "ERAnaNCPi0Debug");

    /// Default destructor
    virtual ~ERAnaNCPi0Debug() {}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout = nullptr);

  private:

    TTree* _tree; 
    int _parentPDG;           /// true PDG of parent of the electron (only for running on MC)
    int _mcPDG;               /// true PDG of "single electron" (probably 11 or 22)
    double _e_Edep;           /// Electron's truth energy
    double _dedx;             /// dedx of "single electron" shower
    int _n_ertool_showers;
    double _dist_to_closest_track_start;
    ertool::Shower singleE_shower;

    ::geoalgo::GeoAlgo _geoalg;
     AlgoFindRelationship _findRel;

  };
}
#endif

/** @} */ // end of doxygen group
