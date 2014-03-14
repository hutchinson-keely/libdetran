//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  EigenSolver.cc
 *  @brief EigenSolver member definitions
 *  @note  Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#include "EigenSolver.hh"
#include "callow/preconditioner/PCMatrix.hh"
// solvers
#include "PowerIteration.hh"
#include "SlepcSolver.hh"
#include "Davidson.hh"
#include "Eispack.hh"

namespace callow
{

//----------------------------------------------------------------------------//
void InitEigenSolverFactory()
{
  REGISTER_CLASS(EigenSolver, PowerIteration, "power")
  REGISTER_CLASS(EigenSolver, Davidson, "davidson")
  REGISTER_CLASS(EigenSolver, Eispack, "eispack")
#ifdef DETRAN_ENABLE_SLEPC
  REGISTER_CLASS(EigenSolver, SlepcSolver, "slepc")
#endif
}

//----------------------------------------------------------------------------//
EigenSolver::SP_solver EigenSolver::Create(SP_db db)
{
  InitEigenSolverFactory();
  // get key
  std::string key = "power";
  if (db)
  {
    if (db->check("eigen_solver_type"))
      key = db->get<std::string>("eigen_solver_type");
  }
  else
  {
    db = new detran_utilities::InputDB();
    db->put<std::string>("eigen_solver_type", key);
  }
  return (Factory_T::Instance().GetCreateFunction(key))(db);
}

//----------------------------------------------------------------------------//
EigenSolver::EigenSolver(const std::string &name, SP_db db)
  : d_db(db)
  , d_name(name)
  , d_tolerance(1e-5)
  , d_maximum_iterations(1000)
  , d_monitor_level(2)
  , d_number_iterations(0)
  , d_lambda(0.0)
  , d_status(RUNNING)
{
  Require(d_db);
  set_parameters(d_db);
}

//----------------------------------------------------------------------------//
void EigenSolver::set_operators(SP_matrix A, SP_matrix B)
{
  Insist(A, "The operator A cannot be null");
  d_A = A;
  Ensure(d_A->number_rows() == d_A->number_columns());

  // Setup linear system if this is a generalized eigenvalue problem
  if (B)
  {
    d_B = B;
    SP_db linsys_db;
    Ensure(d_B->number_rows() == d_B->number_columns());
    Ensure(d_B->number_rows() == d_A->number_columns());
    if (d_db->check("eigen_solver_linear_solver_db"))
    {
      linsys_db = d_db->get<SP_db>("eigen_solver_linear_solver_db");
    }
    d_solver = LinearSolver::Create(linsys_db);
    d_solver->set_operator(d_B);
  }
}

//----------------------------------------------------------------------------//
void EigenSolver::set_preconditioner(SP_preconditioner P)
{
  Insist(P, "The preconditioner must not be NULL.");
  Insist(d_solver, "Preconditioner makes no sense for standard EVPs.");
  d_solver->set_preconditioner(P);
}

//----------------------------------------------------------------------------//
void EigenSolver::set_preconditioner_matrix(SP_matrix P)
{
  Insist(P, "Matrix must not be NULL in order to build a preconditioner.");
  SP_preconditioner PC(new PCMatrix(P, d_db));
  d_solver->set_preconditioner(PC);
}

//----------------------------------------------------------------------------//
void EigenSolver::set_parameters(SP_db db)
{
  Require(db);
  d_db = db;
  if (d_db->check("eigen_solver_tol"))
    d_tolerance = d_db->get<double>("eigen_solver_tol");
  if (d_db->check("eigen_solver_maxit"))
    d_maximum_iterations = d_db->get<int>("eigen_solver_maxit");
  if (db->check("eigen_solver_monitor_level"))
    d_monitor_level = d_db->get<int>("eigen_solver_monitor_level");

  d_number_iterations = 0;
  d_residual_norm.resize(d_maximum_iterations, 0.0);

  Ensure(d_tolerance >= 0.0);
  Ensure(d_maximum_iterations > 0);
}


//----------------------------------------------------------------------------//
int EigenSolver::solve(Vector &x, Vector &x0)
{
  Require(x.size() == d_A->number_rows());
  if (x0.size())
    Require(x0.size() == d_A->number_rows());
  d_status = MAXIT;
  solve_impl(x, x0);
  if (d_status ==  MAXIT && d_monitor_level > 0)
  {
    printf("*** %s did not converge within the maximum number of iterations\n",
           d_name.c_str());
  }
  return d_status;
}

//----------------------------------------------------------------------------//
bool EigenSolver::monitor(int it, double l, double r)
{
  // record the iteration and residual norm
  d_number_iterations = it;
  d_residual_norm[it] = r;
  // echo the residual
  if (d_monitor_level > 1)
  {
    printf("iteration: %5i  eigenvalue: %12.8e    residual: %12.8e \n",
           it, l, r);
  }
  // send a signal
  if (it == d_maximum_iterations)
  {
    d_status = MAXIT;
    return true;
  }
  if (r < d_tolerance)
  {
    if (d_monitor_level > 0)
    {
      printf("*** %s converged in %5i iterations with a residual of %12.8e \n",
             d_name.c_str(), it, r);
    }
    d_status = SUCCESS;
    return true;
  }
  return false;
}

} // end namespace callow

//----------------------------------------------------------------------------//
//              end of file EigenSolver.cc
//----------------------------------------------------------------------------//
