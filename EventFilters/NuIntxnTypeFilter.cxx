#ifndef LARLITE_NUINTXNTYPEFILTER_CXX
#define LARLITE_NUINTXNTYPEFILTER_CXX

#include "NuIntxnTypeFilter.h"
#include "DataFormat/mctruth.h"

namespace larlite {
  
  /// Constructor
  NuIntxnTypeFilter::NuIntxnTypeFilter(){ 
    std::cout<<"constructor!"<<std::endl;
    _name="NuIntxnTypeFilter"; 
    _fout=0;
    _desired_mode = NuIntxnTypeFilter::kINVALID_MODE;
    _desired_nu   = NuIntxnTypeFilter::kINVALID_NU;
  }
  

  bool NuIntxnTypeFilter::initialize() {
    std::cout<<"initialize!"<<std::endl;
    print(larlite::msg::kNORMAL,_name,Form("Desired mode set to %d, desired neutrino set to %d.",_desired_mode,_desired_nu));
    
    /// For now, if user didn't set mode AND nu, complain.
    if ( _desired_mode == NuIntxnTypeFilter::kINVALID_MODE ||
	 _desired_nu == NuIntxnTypeFilter::kINVALID_NU ){
      print(larlite::msg::kERROR,_name,Form("You didn't set a desired mode or a desired neutrino type!"));
      return false;
    }
    
    total_evts = 0;
    kept_evts  = 0;

    return true;
  }
  
  bool NuIntxnTypeFilter::analyze(storage_manager* storage) {

    total_evts++;

    auto ev_mctruth = storage->get_data<event_mctruth>("generator");    

    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }
    if(!ev_mctruth->size()) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("MCTruth has zero size?! Maybe you're using outdated dataformats."));
      return false;
    }
    if(ev_mctruth->size() != 1) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("MCTruth has size more than 1? Why are there multiple neutrinos in this event?!"));
      return false;
    }

    auto mctruth = ev_mctruth->at(0);

    if( mctruth.GetNeutrino().Mode() != _desired_mode )
      return false;
    
    if( mctruth.GetNeutrino().Nu().PdgCode() != _desired_nu )
      return false;
    
    kept_evts++;

    return true;
  }

  bool NuIntxnTypeFilter::finalize() {

    print(larlite::msg::kNORMAL,_name,Form("Total events = %zu, kept events = %zu.\n",total_evts,kept_evts));
      
    return true;
  }

}
#endif
