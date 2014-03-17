//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  SlepcSolver.hh
 *  @brief SlepcSolver class definition
 *  @note  Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_SLEPCSOLVER_HH_
#define callow_SLEPCSOLVER_HH_

#include "EigenSolver.hh"

namespace callow
{

/**
 *  @class SlepcSolver
 *  @brief Solve the eigenvalue problem with SLEPc
 */
class SlepcSolver: public EigenSolver
{

public:

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  SlepcSolver(SP_db db);

  virtual ~SlepcSolver();

  //--------------------------------------------------------------------------//
  // PUBLIC FUNCTIONS
  //--------------------------------------------------------------------------//

  /// Sets the operators for the problem.
  void set_operators(SP_matrix  A,
                     SP_matrix  B  = SP_matrix(0));


  /// Set the preconditioner for a generalized eigenvalue problem
  void set_preconditioner(SP_preconditioner pc);

  /// Set the preconditioner matrix for a generalized eigenvalue problem
  void set_preconditioner_matrix(SP_matrix P);

  /// Set the EPS type
  void set_eps_type(const std::string &eps_type);

private:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  /// SLEPc solver type
  std::string d_eps_type;
  /// SLEPc solver object
  EPS d_slepc_solver;
  /// Optional preconditioner
  SP_preconditioner d_P;

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL EIGENSOLVERS MUST IMPLEMENT THIS
  //--------------------------------------------------------------------------//

  virtual void solve_impl(Vector &x, Vector &x0);

};

} // end namespace callow

#endif /* callow_SLEPCSOLVER_HH_ */

//----------------------------------------------------------------------------//
//              end of file SlepcSolver.hh
//----------------------------------------------------------------------------//
