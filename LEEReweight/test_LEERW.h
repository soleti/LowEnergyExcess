/**
 * \file test_LEERW.h
 *
 * \ingroup LEEReweight
 *
 * \brief Class def header for a class test_LEERW
 *
 * @author davidkaleko
 */

/** \addtogroup LEEReweight

    @{*/

#ifndef LARLITE_TEST_LEERW_H
#define LARLITE_TEST_LEERW_H

#include "Analysis/ana_base.h"
#include "LEERW.h"

namespace larlite {
/**
   \class test_LEERW
   User custom analysis class made by SHELL_USER_NAME
 */
class test_LEERW : public ana_base {

public:

  /// Default constructor
  test_LEERW() {
    _name = "test_LEERW";
    _fout = 0;
    initial_nu_spectrum = 0;
    reweighted_nu_spectrum = 0;
    initial_evis_uz_corr = 0;
    sculpted_evis_uz_corr = 0;
    initial_electron_spectrum = 0;
    reweighted_electron_spectrum = 0;
  }

  /// Default destructor
  virtual ~test_LEERW() {}

  virtual bool initialize();

  virtual bool analyze(storage_manager* storage);

  virtual bool finalize();

protected:

  ::lee::LEERW _rw;

  double summed_sculpting_weight;

  TH1F* initial_nu_spectrum;
  TH1F* reweighted_nu_spectrum;
  TH1F* initial_electron_spectrum;
  TH1F* reweighted_electron_spectrum;
  TH2D* initial_evis_uz_corr;
  TH2D* sculpted_evis_uz_corr;

};
}
#endif

//**************************************************************************
//
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group
