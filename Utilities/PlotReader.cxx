#ifndef PLOTREADER_CXX
#define PLOTREADER_CXX

#include "PlotReader.h"

namespace lee {

namespace util {

PlotReader* PlotReader::_me = 0;

TObject* PlotReader::GetObject() {

	TObject* result = NULL;

	if (_filename.empty() || _objectname.empty()) {
		print(::larlite::msg::kERROR, __FUNCTION__, "ERROR: PlotReader needs you to set filename and object name.");
		return result;
	}

	TFile* f = new TFile(_filename.c_str(), "READ");

	//Some basic checks (file exists, object in file exists, correct type)
	if (!f) {
		print(::larlite::msg::kERROR, __FUNCTION__, Form("ERROR: File %s does not exist!",_filename.c_str()));

		return result;
	}
	if (!f->GetListOfKeys()->Contains(_objectname.c_str())) {
		print(::larlite::msg::kERROR, __FUNCTION__, Form("ERROR: File %s does not contain object %s!",_filename.c_str(),_objectname.c_str()));
		return result;
	}

	// Actually get the object
	result = (TObject*)f->Get(_objectname.c_str());

	return result;

}

}//end namespace util
}//end namespace lee
#endif
