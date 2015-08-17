/**
 * \file NuIntxnTypeFilter.h
 *
 * \ingroup EventFilters
 * 
 * \brief Class def header for a class NuIntxnTypeFilter
 *
 * @author davidkaleko
 */

/** \addtogroup EventFilters

    @{*/

#ifndef LARLITE_NUINTXNTYPEFILTER_H
#define LARLITE_NUINTXNTYPEFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class NuIntxnTypeFilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class NuIntxnTypeFilter : public ana_base{
  
  public:

    /// Default constructor
    NuIntxnTypeFilter();

    /// Default destructur
    virtual ~NuIntxnTypeFilter(){}

    /** IMPLEMENT in NuIntxnTypeFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NuIntxnTypeFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NuIntxnTypeFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    enum Mode_t{
      kQE = 0,
      kResonant = 1,
      kDIS = 2,
      kINVALID_MODE = -1
    };

    enum Nu_t{
      kNue = 12,
      kNumu = 14,
      kINVALID_NU = -1
    };
    
    void setDesiredMode(Mode_t mode){ _desired_mode = mode; }
    void setDesiredNu(Nu_t nu)      {   _desired_nu = nu;   }

  protected:

    Mode_t _desired_mode;
    Nu_t   _desired_nu;

    size_t total_evts;
    size_t kept_evts;
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
