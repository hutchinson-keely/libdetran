//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  MR1.hh
 *  @brief MR1 class definition
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_MR1_HH_
#define callow_MR1_HH_

#include "LinearSolver.hh"

namespace callow
{

/**
 *  @class MR1
 *  @brief One-step, minimum residual iteration
 *
 *  Given an iterate \f$ x \f$, the residual is defined
 *  @f[
 *     r = b - \mathbf{A}x \, .
 *  @f]
 *  Then, we define
 *  @f[
 *     p = \mathbf{A}r
 *  @f]
 *  and
 *  @f[
 *     a = (p^T r) / (p^T p) \, ,
 *  @f]
 *  which give the new iterate
 *  @f[
 *     x = x + a r
 *  @f]
 *  and updated residual
 *  @f[
 *     r = r - a p \, .
 *  @f]
 *
 *  This method requires that \f$ \mathbf{A} \f$ is positive definite, but not
 *  necessarily symmetric.
 *  Either left or right preconditioning is allowed if the resulting
 *  operator remains positive definite.
 *
 *  Reference:
 *    Y. Saad, Iterative Methods for Sparse Linear Systems, pp 140-141.
 */
class MR1 : public LinearSolver
{

public:

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  MR1(SP_db db);

  virtual ~MR1();

private:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  // working vector
  SP_vector d_z;

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL LINEAR SOLVERS MUST IMPLEMENT THIS
  //--------------------------------------------------------------------------//

  /**
   *  @param b  right hand side
   *  @param x  unknown vector
   */
  void solve_impl(const Vector &b, Vector &x);

  /// Applies the (possibly) preconditioned operator
  void apply_preconditioned_matrix(const Vector &v_in, Vector &v_out);

};

} // end namespace callow

#endif /* callow_MR1_HH_ */
