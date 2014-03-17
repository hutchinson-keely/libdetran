//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  LinearSolver.hh
 *  @brief LinearSolver class definition
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_LINEARSOLVER_HH_
#define callow_LINEARSOLVER_HH_

#include "callow/callow_config.hh"
#include "callow/utils/CallowDefinitions.hh"
#include "callow/matrix/MatrixBase.hh"
#include "callow/preconditioner/Preconditioner.hh"
#include "utilities/Factory.hh"
#include "utilities/InputDB.hh"
#include "utilities/SP.hh"
#include <cstdio>
#include <string>
#include <vector>

namespace callow
{

/**
 *  @class LinearSolver
 *  @brief Base class for iterative linear solvers
 *
 *  We solve problems of the form
 *  @f[
 *      \mathbf{A}x = b
 *  @f]
 *  via iterative methods.  A system is "solved" when the
 *  norm of the residual is small enough or some maximum
 *  iteration count is reached.  The residual is defined
 *  @f[
 *      \mathbf{A}x - b
 *  @f]
 *  and its norm is
 *  @f[
 *    r =  || \mathbf{A}x - b ||
 *  @f]
 *  By default, the L2 norm is used, though L1 and Linf
 *  are also recorded can can be used.  This represents
 *  the absolute norm.  Sometimes it makes more sense to
 *  check the residual with respect to the initial norm,
 *  in which case the relative norm is
 *  @f[
 *      r_n / r_0 = || \mathbf{A}x^{n} - b || / || \mathbf{A}x^{0} - b ||
 *  @f]
 *  The iteration terminates when
 *  @f[
 *      r_n < \mathrm{max} ( \tau_{\mathrm{rel}} r_0, \tau_{\mathrm{abs}} )
 *  @f]
 *
 *  The solvers currently implemented are
 *    - Richardson
 *    - Jacobi
 *    - Gauss-Seidel
 *    - GMRES(m)
 *  along with Jacobi and ILU0 preconditioners.  If PETSc is enabled,
 *  all of its solvers are potentially available.
 *
 *  Note, some linear solvers require that the matrix provides L, U, and
 *  D operations.  Here, we simply require those solvers to have a \ref
 *  Matrix operator or subclasses so that the elements can be accessed
 *  directly.
 *
 *  Relevant database parameters include
 *   @param linear_solver_atol   absolute tolerance (||r_n|| < atol)
 *   @param linear_solver_rtol   relative tolerance (||r_n|| < rtol * ||r_0||)
 *   @param linear_solver_maxit  maximum iterations (n < maxit)
 */

class CALLOW_EXPORT LinearSolver
{

public:

  //--------------------------------------------------------------------------//
  // ENUMERATIONS
  //--------------------------------------------------------------------------//

  enum pcside
  {
    NONE, LEFT, RIGHT
  };

  //--------------------------------------------------------------------------//
  // TYPEDEFS
  //--------------------------------------------------------------------------//

  typedef detran_utilities::SP<LinearSolver>        SP_solver;
  typedef MatrixBase::SP_matrix                     SP_matrix;
  typedef Preconditioner::SP_preconditioner         SP_preconditioner;
  typedef Vector::SP_vector                         SP_vector;
  typedef detran_utilities::InputDB::SP_input       SP_db;

  // REQUIRED type defining the creation function
  typedef SP_solver (*CreateFunction)(SP_db db);

  // Factory
  typedef detran_utilities::Factory<LinearSolver>   Factory_T;

  virtual ~LinearSolver(){}

  static SP_solver Create(SP_db db);

  //--------------------------------------------------------------------------//
  // PUBLIC FUNCTIONS
  //--------------------------------------------------------------------------//

  /// Reset the solver parameters.  This also resets residuals, counts, etc.
  void set_parameters(SP_db db);

  /**
   *  @brief Set the linear system operator.
   *
   *  This sets a new operator and, if given, a new preconditioner.  If a
   *  preconditioner is not provided, the current preconditioner is
   *  voided.
   *
   *  @param  A     linear system matrix
   *  @param  P     preconditioner (optional)
   */
  virtual void set_operator(SP_matrix A,
                            SP_preconditioner P = SP_preconditioner(0));

  /**
   *  @brief Set the preconditioner.
   *
   *  This sets the user-provided preconditioner.  If no preconditioner is
   *  passed, then a preconditioner based on the operator A will be
   *  constructed based on the parameter database.
   *
   *  @param P      preconditioner
   */
  virtual void set_preconditioner(SP_preconditioner P = SP_preconditioner(0));

  /// Get the operator
  SP_matrix get_operator()
  {
    return d_A;
  }

  /// Get the preconditioner
  SP_preconditioner get_preconditioner()
  {
    return d_P;
  }

  /// Get the parameters
  SP_db get_parameters()
  {
    return d_db;
  }

  /// Set the solver status
  void set_status(const int s)
  {
    d_status = s;
  }

  /**
   *  @param b  right hand side
   *  @param x  unknown vector
   */
  int solve(const Vector &b, Vector &x);

  /// return the residual norms
  std::vector<double> residual_norms()
  {
    return d_residual;
  }

  /// return the number of iterations
  int number_iterations() const
  {
    return d_number_iterations;
  }

  void display() const
  {
    d_db->display();
  }

protected:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  /// Solver name (e.g. richardson)
  std::string d_name;
  /// Parameter database
  SP_db d_db;
  /// Absolute tolerance on residual norm
  double d_absolute_tolerance;
  /// Relative tolerance on residual norm
  double d_relative_tolerance;
  /// Maximum number of iterations
  int d_maximum_iterations;
  /// Vector of residual norms at each iteration
  std::vector<double> d_residual;
  /// Number of iterations performed
  int d_number_iterations;
  /// Linear operator
  SP_matrix d_A;
  /// Preconditioner
  SP_preconditioner d_P;
  /// Preconditioner side (one only, and left by default)
  int d_pc_side;
  /// Monitor level (0 = none, higher yields more output)
  int d_monitor_level;
  /// Check whether the residuals grow
  bool d_monitor_diverge;
  /// Which vector norm to use?
  int d_norm_type;
  /// Relaxation parameter
  double d_omega;
  /// Use norm of successive iterates
  bool d_successive_norm;

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  LinearSolver(std::string key, SP_db db = SP_db(0));

  //--------------------------------------------------------------------------//
  // IMPLEMENTATION
  //--------------------------------------------------------------------------//

  /// print out iteration and residual for initial
  virtual bool monitor_init(double r);

  // print out iteration and residual
  virtual bool monitor(int it, double r);

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL LINEAR SOLVERS MUST IMPLEMENT THIS
  //--------------------------------------------------------------------------//

  virtual void solve_impl(const Vector &b, Vector &x) = 0;

private:

  int d_status;


};

CALLOW_TEMPLATE_EXPORT(detran_utilities::SP<LinearSolver>)

/// Creation function template
template <typename D>
LinearSolver::SP_solver
Create(LinearSolver::SP_db db)
{
  return LinearSolver::SP_solver(new D(db));
}

} // end namespace callow

#endif /* callow_LINEARSOLVER_HH_ */

//----------------------------------------------------------------------------//
//              end of file LinearSolver.hh
//----------------------------------------------------------------------------//
