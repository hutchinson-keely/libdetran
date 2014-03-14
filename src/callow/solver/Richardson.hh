//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  Richardson.hh
 *  @brief Richardson class definition
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_RICHARDSON_HH_
#define callow_RICHARDSON_HH_

#include "LinearSolver.hh"

namespace callow
{

/**
 *  @class Richardson
 *  @brief Uses (modified) Richardson iteration to solve a system
 *
 *  Richardson iteration solves a linear system via the
 *  process
 *  @f[
 *     x^{(n+1)} = (\mathbf{I - \omega A})x^{(n)} + \omega b
 *  @f]
 *  where \f$ \omega \f$ is something like a relaxation factor
 *  that takes on values between (roughly) 0 and 2.  By default,
 *  \f$ \omega = 1 \f$.
 *
 */
class Richardson: public LinearSolver
{

public:

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  Richardson(SP_db);

  virtual ~Richardson(){}

private:

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL LINEAR SOLVERS MUST IMPLEMENT THIS
  //--------------------------------------------------------------------------//

  /**
   *  @param b  right hand side
   *  @param x  unknown vector
   */
  void solve_impl(const Vector &b, Vector &x);

};

} // end namespace callow

#endif /* callow_RICHARDSON_HH_ */
