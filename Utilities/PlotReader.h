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
  // TObject* GetObject();

  // Change function calls like
  // T* ptr;
  // GetObject(ptr)
  // ... to ...
  // T obj
  // GetObject(obj)
  template <class T>
  void GetObject(T& obj) {

    if (_filename.empty() || _objectname.empty()) {
      print(::larlite::msg::kERROR, __FUNCTION__, "ERROR: PlotReader needs you to set filename and object name.");
      return;
    }

    TFile* f = TFile::Open(_filename.c_str(), "READ");

    //Some basic checks (file exists, object in file exists, correct type)
    if (!f) {
      print(::larlite::msg::kERROR, __FUNCTION__, Form("ERROR: File %s does not exist!",_filename.c_str()));
      return;
    }
    if (!f->GetListOfKeys()->Contains(_objectname.c_str())) {
      print(::larlite::msg::kERROR, __FUNCTION__, Form("ERROR: File %s does not contain object %s!",_filename.c_str(),_objectname.c_str()));
      return;
    }

  // Actually get the object
    auto result = (T*)f->Get(_objectname.c_str());
    obj = *result;
  //TObject * finalresult = new TObject();
  //result->Copy(*finalresult);
    f->Close();
    delete f;
  //return finalresult;

  }

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

