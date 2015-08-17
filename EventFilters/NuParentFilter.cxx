#ifndef LARLITE_NUPARENTFILTER_CXX
#define LARLITE_NUPARENTFILTER_CXX

#include "NuParentFilter.h"
#include "DataFormat/mcflux.h"

namespace larlite {
  
  /// Constructor
  NuParentFilter::NuParentFilter(){ 
    _name="NuParentFilter"; 
    _fout=0;
    _desired_parent = NuParentFilter::kINVALID_PARENT;
    _desired_nu   = NuParentFilter::kINVALID_NU;
  }
  

  bool NuParentFilter::initialize() {

    print(larlite::msg::kNORMAL,_name,Form("Desired parent set to %d, desired neutrino set to %d.",_desired_parent,_desired_nu));
    
    /// For now, if user didn't set parent AND nu, complain.
    if ( _desired_parent == NuParentFilter::kINVALID_PARENT ||
	 _desired_nu == NuParentFilter::kINVALID_NU ){
      print(larlite::msg::kERROR,_name,Form("You didn't set a desired parent or a desired neutrino type!"));
      return false;
    }
    
    total_evts = 0;
    kept_evts  = 0;

    return true;
  }
  
  bool NuParentFilter::analyze(storage_manager* storage) {

    total_evts++;

    auto ev_mcflux = storage->get_data<event_mcflux>("generator");    

    if(!ev_mcflux) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mcflux!"));
      return false;
    }
    if(!ev_mcflux->size()) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("MCFlux has zero size?! Maybe you're using outdated dataformats."));
      return false;
    }
    if(ev_mcflux->size() != 1) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("MCFlux has size more than 1? Why are there multiple neutrinos in this event?!"));
      return false;
    }

    auto mcflux = ev_mcflux->at(0);
    int nu_type = mcflux.fntype;
    int nu_par  = mcflux.fndecay;
    
    if( abs(nu_type) != _desired_nu )
      return false;
    
    if(_desired_parent == NuParentFilter::kPion){
      if(nu_par != 13 && nu_par != 14)
	return false;
    }
    else if (_desired_parent == NuParentFilter::kMuon){
      if(nu_par != 11 && nu_par != 12)
	return false;
    }
    else if (_desired_parent == NuParentFilter::kKaon){
      if(nu_par > 10 || nu_par < 1)
	return false;
    }
    else
      print(larlite::msg::kERROR,_name,Form("You didn't set a desired parent type!"));
    
    kept_evts++;

    return true;
  }

  bool NuParentFilter::finalize() {

    print(larlite::msg::kNORMAL,_name,Form("Total events = %zu, kept events = %zu.\n",total_evts,kept_evts));
      
    return true;
  }

}
#endif
