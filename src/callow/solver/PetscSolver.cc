//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  PetscSolver.cc
 *  @brief PetscSolver member definitions
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#include "callow_config.hh"

#ifdef CALLOW_ENABLE_PETSC

#include "PetscSolver.hh"

namespace callow
{

//----------------------------------------------------------------------------//
PetscSolver::PetscSolver(SP_db db)
  : LinearSolver("petsc", db)
{
  // create the petsc object
  PetscErrorCode ierr = KSPCreate(PETSC_COMM_SELF, &d_petsc_solver);
  Insist(!ierr, "Error creating KSP object.");

  // set the convergence criteria (third param is divergence tolerace; skip)
  ierr = KSPSetTolerances(d_petsc_solver,
                          d_relative_tolerance,
                          d_absolute_tolerance,
                          PETSC_DEFAULT,
                          d_maximum_iterations);
  Insist(!ierr, "Error setting KSP tolerances.");

  /// set the monitor so we can extract residuals on-the-fly
  ierr = KSPMonitorSet(d_petsc_solver, petsc_ksp_monitor, this, PETSC_NULL);
  Insist(!ierr, "Error setting KSP monitor.");

  ierr = KSPSetInitialGuessNonzero(d_petsc_solver, PETSC_TRUE);
  Insist(!ierr, "Error setting KSP nonzeros initial guess.");
}

//----------------------------------------------------------------------------//
PetscSolver::~PetscSolver()
{
  KSPDestroy(&d_petsc_solver);
}

//----------------------------------------------------------------------------//
void PetscSolver::set_operator(SP_matrix A, SP_preconditioner P)
{
  LinearSolver::set_operator(A, P);

  set_preconditioner(d_P);

  // Set the operator.  Note, this structure really eliminates
  // the option of using a second matrix for preconditioning.
  // That's fine, though, since any non-A pc's will be shell
  // operations, which are independent of this choice.
  PetscErrorCode ierr = KSPSetOperators(d_petsc_solver,
                                        d_A->petsc_matrix(),
                                        d_A->petsc_matrix(),
                                        SAME_NONZERO_PATTERN);

  Ensure(!ierr);
}

//----------------------------------------------------------------------------//
void PetscSolver::set_preconditioner(SP_preconditioner P)
{
  // Set a callow preconditioner, if applicable
  LinearSolver::set_preconditioner(P);

  // Provide PETSc the callow preconditioner, or use a PETSc preconditioner
  PC pc;
  PetscErrorCode ierr = KSPGetPC(d_petsc_solver, &pc);
  if (d_P)
  {
    ierr = PCSetType(pc, PCSHELL);
    d_P->set_petsc_pc(pc);
  }
  else
  {
    // If we have a PETSC solver and no callow PC, we assume a PETSc
    // PC is requested
    std::string petsc_pc_type;
    if (d_db->check("petsc_pc_type"))
      petsc_pc_type = d_db->get<std::string>("petsc_pc_type");
    if (petsc_pc_type == "ilu")
    {
      PCSetType(pc, PCILU);
      int levels = 2;
      if (d_db->check("petsc_pc_factor_levels"))
        levels = d_db->get<int>("petsc_pc_factor_levels");
      PCFactorSetLevels(pc, levels);
      // Fill zeros on the diagonal, even if they wouldn't be
      PCFactorSetAllowDiagonalFill(pc);
    }
    else if (petsc_pc_type == "lu")
    {
      PCSetType(pc, PCLU);
      PCFactorSetReuseOrdering(pc, PETSC_TRUE);
      PCFactorSetReuseFill(pc, PETSC_TRUE);
      // PCFactorSetMatOrderingType(pc, MATORDERINGND);
      // PCFactorSetMatSolverPackage(pc, MATSOLVERSUPERLU);
    }
    else
    {
      PCSetType(pc, PCNONE);
    }
    PCSetFromOptions(pc);
  }

  // Set the preconditioner side.
  Assert(d_pc_side == LEFT || d_pc_side == RIGHT);
  if (d_pc_side == LEFT)
    ierr = KSPSetPCSide(d_petsc_solver, PC_LEFT);
  else
    ierr = KSPSetPCSide(d_petsc_solver, PC_RIGHT);

  Ensure(!ierr);
}

} // end namespace callow

#endif

//----------------------------------------------------------------------------//
//              end of file PetscSolver.cc
//----------------------------------------------------------------------------//
