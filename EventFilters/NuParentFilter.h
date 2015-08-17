/**
 * \file NuParentFilter.h
 *
 * \ingroup EventFilters
 * 
 * \brief Class def header for a class NuParentFilter
 *
 * @author davidkaleko
 */

/** \addtogroup EventFilters

    @{*/

#ifndef LARLITE_NUPARENTFILTER_H
#define LARLITE_NUPARENTFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class NuParentFilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class NuParentFilter : public ana_base{
  
  public:

    /// Default constructor
    NuParentFilter();

    /// Default destructur
    virtual ~NuParentFilter(){}

    /** IMPLEMENT in NuParentFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NuParentFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NuParentFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    enum NuParent_t{
      kPion = 0,
      kMuon,
      kKaon,
      kINVALID_PARENT
    };

    enum Nu_t{
      kNue = 12,
      kNumu = 14,
      kINVALID_NU = -1
    };
    
    void setDesiredParent(NuParent_t parent){ _desired_parent = parent; }
    void setDesiredNu(Nu_t nu)              {     _desired_nu = nu;     }
 
  protected:

    NuParent_t _desired_parent;
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
