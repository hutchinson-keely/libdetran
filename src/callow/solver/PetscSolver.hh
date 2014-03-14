//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  PetscSolver.hh
 *  @brief PetscSolver class definition
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_PETSCSOLVER_HH_
#define callow_PETSCSOLVER_HH_

#ifdef DETRAN_ENABLE_PETSC

#include "LinearSolver.hh"
// preconditioners
#include "callow/preconditioner/PCILU0.hh"
#include "callow/preconditioner/PCJacobi.hh"

namespace callow
{

/**
 *  @class PetscSolver
 *  @brief Uses PETSc to solve a system
 */
class PetscSolver: public LinearSolver
{

public:

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  PetscSolver(SP_db db);

  virtual ~PetscSolver();

  //--------------------------------------------------------------------------//
  // PUBLIC FUNCTIONS
  //--------------------------------------------------------------------------//

  void set_operator(SP_matrix A, SP_preconditioner P = SP_preconditioner(0));

  void set_preconditioner(SP_preconditioner P = SP_preconditioner(0));

  /// Get the KSP object
  KSP petsc_solver(){return d_petsc_solver;}

private:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  // petsc solver type
  KSP d_petsc_solver;

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL LINEAR SOLVERS MUST IMPLEMENT THIS
  //--------------------------------------------------------------------------//

  /**
   *  @param b  right hand side
   *  @param x  unknown vector
   */
  void solve_impl(const Vector &b, Vector &x);

  /// let the monitor wrapper call our monitor
  friend PetscErrorCode
  petsc_ksp_monitor(KSP ksp, PetscInt it, PetscReal rnorm, void* ctx);

};

/// Monitor the solution
PetscErrorCode petsc_ksp_monitor(KSP ksp, PetscInt it,
                                 PetscReal rnorm, void* ctx);

} // end namespace callow

// Inline member definitions
#include "PetscSolver.i.hh"

#endif // DETRAN_ENABLE_PETSC

#endif /* callow_PETSCSOLVER_HH_ */

//----------------------------------------------------------------------------//
//              end of file PetscSolver.hh
//----------------------------------------------------------------------------//
