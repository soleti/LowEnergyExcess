/**
 * \file ERAnaLowEnergyExcess.h
 *
 * \ingroup LowEPlots
 *
 * \brief Class def header for a class ERAnaLowEnergyExcess
 *
 * @author jzennamo
 */

/** \addtogroup LowEPlots

    @{*/

#ifndef ERTOOL_ERAnaLowEnergyExcess_H
#define ERTOOL_ERAnaLowEnergyExcess_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include <string>
#include "DataFormat/mctruth.h"
// #include "ERToolBackend/ParticleID.h"
#include "../../LArLiteApp/fluxRW/fluxRW.h"
#include "GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"
#include "LEERW.h"
#include <cmath>
#include "GeoAlgo/GeoAlgo.h"
#include "ECCQECalculator.h"


namespace ertool {

    /**
       \class ERAnaLowEnergyExcess
       User custom Analysis class made by kazuhiro
     */
    class ERAnaLowEnergyExcess : public AnaBase {

    public:

        /// Default constructor
        ERAnaLowEnergyExcess(const std::string& name = "ERAnaLowEnergyExcess");

        /// Default destructor
        virtual ~ERAnaLowEnergyExcess() {}

        /// Reset function
        virtual void Reset() {}

        /// Function to accept fclite::PSet
        void AcceptPSet(const ::fcllite::PSet& cfg) {}

        /// Called @ before processing the first event sample
        void ProcessBegin();

        /// Function to evaluate input showers and determine a score
        bool Analyze(const EventData &data, const ParticleGraph &ps);

        /// Called after processing the last event sample
        void ProcessEnd(TFile* fout);

        /// setting result tree name for running the LowEExcess plotting code
        void SetTreeName(const std::string& name) { _treename = name; }

        /// set the energy cut to be used when counting particles
        void SetECut(double c) { _eCut = c; }

        // geoalgo::AABox TPC;

        //Set this to true if you're running over LEE sample (so it uses LEE reweighting package)
        void SetLEESampleMode(bool flag) { _LEESample_mode = flag; }

    private:

        // Calc new E_nu^calo, with missing pT cut
        double EnuCaloMissingPt(const std::vector< ::ertool::NodeID_t >& Children, const ParticleGraph &graph);

        // Determine if the event is "simple" (1e, np, 0else)
        bool isInteractionSimple(const Particle &singleE, const ParticleGraph &ps);

        // Result tree comparison for reconstructed events
        TTree* _result_tree;
        std::string _treename;

        float _eCut;

        double _e_nuReco;         /// Neutrino energy
        double _e_dep;            /// Neutrino energy
        double _weight;
        int _numEvts;
        bool _is_fiducial;
        double _ancestor_e;
        int _ancestorPDG;           /// true PDG of parent of the electron (only for running on MC)
        double _parent_e;
        int _parentPDG;           /// true PDG of parent of the electron (only for running on MC)
        int _mcPDG;               /// true PDG of "single electron" (probably 11 or 22)
        int _mcGeneration;        /// True generation of single electron (to characterize cosmics and other backgrounds)
        double _longestTrackLen;  /// longest track associated with the reconstructed neutrino
        double _x_vtx;            /// Neutrino vertex points (x,y,z separated)
        double _y_vtx;
        double _z_vtx;
        double _start_x;            /// Neutrino vertex points (x,y,z separated)
        double _start_y;            /// Neutrino vertex points (x,y,z separated)
        double _start_z;            /// Neutrino vertex points (x,y,z separated)
        double _end_x;            /// Neutrino vertex points (x,y,z separated)
        double _end_y;            /// Neutrino vertex points (x,y,z separated)
        double _end_z;            /// Neutrino vertex points (x,y,z separated)

        double _e_theta;          /// Electron's angle w.r.t/ z- axis
        double _e_phi;            /// Electron's phi angle
        double _e_Edep;           /// Electron's truth energy
        double _e_CCQE;           /// Electron's CCQE energy
        double _nu_p;             /// Neutrino reconstructed momentum magnitude
        double _nu_pt;            /// Component of nu momentum that is transverse (_nu_p*sin(_nu_theta))
        double _nu_theta;         /// Neutrino's reconstructed angle w.r.t. z- axis
        int _n_children;          /// Number of children associated with the neutrino interaction
        bool _is_simple;          /// Whether the interaction is 1e+np+0else (reconstructed)
        double _dedx;             /// dedx of "single electron" shower
        double _flash_time;       /// opflash associated with electron... flash time
        double _summed_flash_PE;  /// total reconstructed PE of the flash
        bool _maybe_pi0_MID;    /// whether the neutrino has a gamma tagged as one of its children
        bool _in_tagger;
        bool _in_tagger_phaseA;
        
        bool _in_ob;
        
        // prepare TTree with variables
        void PrepareTreeVariables();
        /// Function to re-set TTree variables
        void ResetTreeVariables();

        ::fluxRW _fluxRW;

        // ertool_helper::ParticleID singleE_particleID;
        ertool::Shower singleE_shower;

        bool _LEESample_mode = false;

        // Variables for B.I.T.E analysis
        double _dist_2wall ;      /// Electron backwards distance 2 wall
        double _dist_2wall_vtx;   /// Vertex backwards distance 2 wall
        ::geoalgo::AABox _vactive;
        ::geoalgo::AABox _tagger_top_1;
        ::geoalgo::AABox _tagger_top_2;
        ::geoalgo::AABox _tagger_under;
        ::geoalgo::AABox _tagger_pipe_1;
        ::geoalgo::AABox _tagger_pipe_2;
        ::geoalgo::AABox _tagger_ft;
    protected:
        bool Intersect(const ::geoalgo::HalfLine_t&);
        bool IntersectDumb(const TVector3& start, const TVector3& end);
        bool IntersectDumbPhaseA(const TVector3& start, const TVector3& end);
        
        bool OBIntersect(const TVector3& start, const TVector3& end);
        
        ::lee::LEERW _rw;
        ::geoalgo::GeoAlgo _geoalg;
        ::lee::util::ECCQECalculator _eccqecalc;

        
    };
}
#endif

/** @} */ // end of doxygen group
