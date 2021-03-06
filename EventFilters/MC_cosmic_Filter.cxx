#ifndef LARLITE_MC_COSMIC_FILTER_CXX
#define LARLITE_MC_COSMIC_FILTER_CXX

#include "MC_cosmic_Filter.h"

namespace larlite {

bool MC_cosmic_Filter::initialize() {

  return true;
}

bool MC_cosmic_Filter::analyze(storage_manager* storage) {

  auto ev_mctruth_v = storage->get_data<event_mctruth>("generator");
  if (!ev_mctruth_v) {
    print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
    return false;
  }

  bool ret = false;

  for (auto &ev_mctruth : *ev_mctruth_v) {

    //Enforce Cosmic Origins
    if (ev_mctruth.Origin() == simb::Origin_t::kCosmicRay) {ret = true;}
    //Corsika cosmics have origin == 0
    else if (ev_mctruth.Origin() == simb::Origin_t::kUnknown) {
      print(larlite::msg::kWARNING, __FUNCTION__, Form("MCTruth origin unknown! Allowing it to pass cosmic filter."));
      ret = true;
    }
    else {ret = false;}

    if (ret == true) continue;

  }

  return ret;

}

bool MC_cosmic_Filter::finalize() {


  return true;
}

}
#endif
