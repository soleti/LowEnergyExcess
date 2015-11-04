/**
 * \file LEERW.h
 *
 * \ingroup LEEReweight
 *
 * \brief This class gets handed a neutrino interaction and computes a weight.
 *  The weight takes into account the energy and angle of the outgoing electron in the interaction
 *  as well as cross section scalings, flux scalings, etc from MiniBooNE to MicroBooNE.
 *  The idea is the user can generate nue interactions, apply some sort of filter to select one that
 *  pass MiniBooNE event selection cuts (one subevent, etc)
 *  then hand the neutrino interaction to this package to get a weight. Looping over events and
 *  histogramming them with this weight will produce a simulated low energy excess in MicroBooNE.
 *  (of course assuming the excess comes from an excess of nue interactions)
 *
 * @author davidkaleko
 */

/** \addtogroup LEEReweight

    @{*/
#ifndef LEERW_H
#define LEERW_H

#include <iostream>
#include "Utilities/PlotReader.h"
#include "Base/larlite_base.h"
#include "TH1.h"
#include "TGraph.h"

/**
   \class LEERW
   User defined class LEERW ... these comments are used to generate
   doxygen documentation!
 */
namespace lee {

class LEERW : public ::larlite::larlite_base {

public:

	/// Default constructor
	LEERW() {
		_flux_ratio = 0;
	}

	/// Default destructor
	~LEERW() {}

private:

	TGraph* _flux_ratio;

};

} // end namespace lee
#endif
/** @} */ // end of doxygen group

