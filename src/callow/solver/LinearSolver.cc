//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  LinearSolver.cc
 *  @brief LinearSolver member definitions
 *  @note  Copyright (C) Jeremy Roberts 2013
 */
//----------------------------------------------------------------------------//

#include "LinearSolver.hh"
// solvers
#include "Richardson.hh"
#include "Jacobi.hh"
#include "GaussSeidel.hh"
#include "GMRES.hh"
#include "PetscSolver.hh"

namespace callow
{

void InitLinearSolverFactory()
{
  REGISTER_CLASS(LinearSolver, Richardson,       "richardson")
  REGISTER_CLASS(LinearSolver, Jacobi,           "jacobi")
  REGISTER_CLASS(LinearSolver, GaussSeidel,      "gauss-seidel")
  REGISTER_CLASS(LinearSolver, GMRES,            "gmres")
#ifdef DETRAN_ENABLE_PETSC
  REGISTER_CLASS(LinearSolver, PetscSolver,      "petsc")
#endif
}

//----------------------------------------------------------------------------//
LinearSolver::SP_solver LinearSolver::Create(SP_db db)
{
  InitLinearSolverFactory();
  // get key
  std::string key = "gmres";
  if (db)
  {
    if (db->check("linear_solver_type"))
      key = db->get<std::string>("linear_solver_type");
  }
  else
  {
    db = new detran_utilities::InputDB();
    db->put<std::string>("linear_solver_type", key);
  }
  return (Factory_T::Instance().GetCreateFunction(key))(db);
}


//----------------------------------------------------------------------------//
LinearSolver::LinearSolver(std::string key, SP_db db)
  : d_name(key)
  , d_absolute_tolerance(1e-8)
  , d_relative_tolerance(1e-8)
  , d_maximum_iterations(100)
  , d_number_iterations(0)
  , d_pc_side(LEFT)
  , d_monitor_level(0)
  , d_monitor_diverge(true)
  , d_norm_type(L2)
  , d_status(RUNNING)
  , d_omega(1.0)
  , d_successive_norm(false)
{
  set_parameters(db);
}

//----------------------------------------------------------------------------//
void LinearSolver::set_parameters(SP_db db)
{
  Require(db);
  d_db = db;
  if (d_db->check("linear_solver_atol"))
    d_absolute_tolerance = d_db->get<double>("linear_solver_atol");
  if (d_db->check("linear_solver_rtol"))
    d_relative_tolerance = db->get<double>("linear_solver_rtol");
  if (d_db->check("linear_solver_maxit"))
    d_maximum_iterations = d_db->get<int>("linear_solver_maxit");
  if (db->check("linear_solver_monitor_level"))
    d_monitor_level = d_db->get<int>("linear_solver_monitor_level");
  if (d_db->check("linear_solver_monitor_diverge"))
    d_monitor_diverge = d_db->get<int>("linear_solver_monitor_diverge");
  if (d_db->check("linear_solver_successive_norm"))
    d_successive_norm = 0 != d_db->get<int>("linear_solver_successive_norm");
  if (d_db->check("linear_solver_relaxation"))
    d_omega = d_db->get<double>("linear_solver_relaxation");
  if (d_db->check("pc_side"))
    d_pc_side = d_db->get<int>("pc_side");

  d_number_iterations = 0;
  d_residual.resize(d_maximum_iterations, 0.0);

  Ensure(d_absolute_tolerance >= 0.0);
  Ensure(d_relative_tolerance >= 0.0);
  Ensure(d_maximum_iterations >  0);
}

//----------------------------------------------------------------------------//
void LinearSolver::set_operator(SP_matrix A, SP_preconditioner P)
{
  Require(A);
  d_A = A;
  d_P = P;
  Ensure(d_A->number_rows() == d_A->number_columns());
  if (d_P)
  {
    Ensure(d_A->number_rows() == d_P->size());
  }
}

//----------------------------------------------------------------------------//
void LinearSolver::set_preconditioner(SP_preconditioner P)
{
  Insist(d_A, "The operator must be set before the preconditioner.");
  d_P = P;
  if (!d_P)
    d_P = Preconditioner::Create(d_A, d_db);
  if (d_P)
  {
    Ensure(d_A->number_rows() == d_P->size());
  }
}

//----------------------------------------------------------------------------//
bool LinearSolver::monitor_init(double r)
{
  d_residual[0] = r;
  if (d_monitor_level > 1)
    printf("iteration: %5i    residual: %12.8e \n", 0, r);
  if (r < d_absolute_tolerance)
  {
    if (d_monitor_level > 0)
    {
      printf("*** %s converged in %5i iterations with a residual of %12.8e \n",
             d_name.c_str(), 0, r );
    }
    d_status = SUCCESS;
    return true;
  }
  return false;
}

// print out iteration and residual
bool LinearSolver::monitor(int it, double r)
{
  d_number_iterations = it;
  d_residual[it] = r;
  if (d_monitor_level > 1)
    printf("iteration: %5i    residual: %12.8e \n", it, r);
 // Assert(it > 0);
  bool value = false;
  if (r < std::max(d_relative_tolerance * d_residual[0],
                   d_absolute_tolerance))
  {
    if (d_monitor_level)
    {
      printf("*** %s converged in %5i iterations with a residual of %12.8e \n",
             d_name.c_str(), it, r );
    }
    d_status = SUCCESS;
    value = true;
  }
  else if (d_monitor_diverge && it >  1 && r - d_residual[it - 1] > 0.0)
  {
    if (d_monitor_level) printf("*** %s diverged \n", d_name.c_str());
    d_status = DIVERGE;
    value = true;
  }
  else if (it == d_maximum_iterations - 1)
  {
    if (d_monitor_level) printf("*** max it\n");
    d_status = MAXIT;
    value = true;
  }
  else
  {
    d_status = RUNNING;
  }
  return value;
}


//----------------------------------------------------------------------------//
int LinearSolver::solve(const Vector &b, Vector &x)
{
  Require(x.size() == b.size());
  Require(x.size() == d_A->number_rows());
  // Resize the norm
  d_residual.resize(d_maximum_iterations+1, 0.0);
  d_status = RUNNING;
  solve_impl(b, x);
  if (d_status ==  MAXIT && d_monitor_level > 0)
  {
     printf("*** %s did not converge within the maximum number of iterations\n",
            d_name.c_str());
  }
  // Resize the norm
  d_residual.resize(d_number_iterations+1);
  return d_status;
}

} // end namespace callow

//---------------------------------------------------------------------------//
//              end of file LinearSolver.cc
//---------------------------------------------------------------------------//
