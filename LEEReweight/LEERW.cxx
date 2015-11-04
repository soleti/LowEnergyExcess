#ifndef LEERW_CXX
#define LEERW_CXX

#include "LEERW.h"

namespace lee {

bool LEERW::initialize() {

	util::PlotReader::GetME()->SetFileName("test_file_name.root");
//	_my_xsec = (TGraph*)util::PlotReader::GetME()->GetObject();
	std::cout << "hello world" << std::endl;
	return true;
}


}
#endif
