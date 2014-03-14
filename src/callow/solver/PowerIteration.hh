//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  PowerIteration.hh
 *  @brief PowerIteration class definition
 *  @note  Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_POWERITERATION_HH_
#define callow_POWERITERATION_HH_

#include "EigenSolver.hh"

namespace callow
{

/**
 *  @class PowerIteration
 *  @brief Solve the eigenvalue problem with the power method
 */

class PowerIteration: public EigenSolver
{

public:

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  PowerIteration(SP_db db);

  virtual ~PowerIteration(){}

protected:

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL EIGENSOLVERS MUST IMPLEMENT THIS
  //--------------------------------------------------------------------------//

  virtual void solve_impl(Vector &x, Vector &x0);

};

} // end namespace callow

#endif // callow_POWERITERATION_HH_

//----------------------------------------------------------------------------//
//              end of file PowerIteration.hh
//----------------------------------------------------------------------------//
