//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  PCIdentity.hh
 *  @brief PCIdentity class definition
 *  @note  Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_PCIDENTITY_HH_
#define callow_PCIDENTITY_HH_

#include "Preconditioner.hh"

namespace callow
{

/**
 *  @class PCIdentity
 *  @brief Implements an indentity preconditioner (i.e. no preconditioning)
 *
 *  This is mostly for testing purposes.
 */
class PCIdentity: public Preconditioner
{

public:

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  PCIdentity(SP_matrix A, SP_db db = SP_db(0))
    : Preconditioner("PCIdentity", db)
  {/* ... */}

  virtual ~PCIdentity(){};

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL PRECONDITIONERS MUST IMPLEMENT THIS
  //--------------------------------------------------------------------------//

  void apply(Vector &b, Vector &x)
  {
    x.copy(b);
  }

};

} // end namespace callow

#endif /* callow_PCIDENTITY_HH_ */
