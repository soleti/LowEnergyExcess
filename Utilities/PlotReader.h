/**
 * \file PlotReader.h
 *
 * \ingroup LEE
 *
 * \brief This opens up TFiles and reads in anything that inherits from TObject, including TGraphs, TH1D, TH2F, TLegend, etc. and returns them
 *
 * @author davidkaleko
 */

/** \addtogroup LEE

    @{*/
#ifndef PLOTREADER_HH
#define PLOTREADER_HH

// #include "FMWKBase/FMWKBase.h"
// #include "FMWKBase/FMWKException.h"
#include "TObject.h"
#include "TFile.h"
#include "Base/larlite_base.h"

/**
   \class PlotReader
   This opens up TFiles and reads in anything that inherits from TObject, including TGraphs, TH1D, TH2F, TLegend, etc. and returns them

 */

namespace lee {

namespace util {

//Inherits from larlite_base just for the pretty "print" functionality
class PlotReader : public ::larlite::larlite_base {

public:

  void SetFileName(std::string filename) { _filename = filename; }

  void SetObjectName(std::string objectname) { _objectname = objectname; }

  void Reset() {
    _filename = "";
    _objectname = "";
  }

  /// Function to get an object from a file
  /// (whoever gets the object should cast it to the appropriate type)
  TObject* GetObject();

  //singleton getter?!?!?!
  static PlotReader* GetME() {
    if (!_me) _me = new PlotReader;
    return _me;
  }

private:
  //singleton!?!
  static PlotReader* _me;

  //constructor is private for singletons right?
  /// Default constructor
  PlotReader() {
    _filename = "";
    _objectname = "";
  }

  /// Default destructor
  virtual ~PlotReader() {};

protected:

  std::string _filename;
  std::string _objectname;

};

}//end namespace util

}//end namespace lee

#endif
/** @} */ // end of doxygen group

