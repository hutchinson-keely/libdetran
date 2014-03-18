//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  MR1.cc
 *  @brief MR1 class definition
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#include "MR1.hh"

namespace callow
{

//----------------------------------------------------------------------------//
MR1::MR1(SP_db db)
  : LinearSolver("mr1", db)
{
  /* ... */
}

//----------------------------------------------------------------------------//
MR1::~MR1()
{
  /* ... */
}

//----------------------------------------------------------------------------//
void MR1::solve_impl(const Vector &b, Vector &x)
{
  // allocate working vectors
  Vector r(x.size(), 0.0);
  Vector p(x.size(), 0.0);
  if (d_P) d_z = new Vector(x);

  // initial residual
  d_A->multiply(x, r);
  r.subtract(b);
  if (d_P && d_pc_side == LEFT)
  {
    d_z->copy(r);
    d_P->apply(*d_z, r);
  }

  // solve the system
  if (monitor(r.norm(), 0)) return;

  for (int iteration = 1; iteration <= d_maximum_iterations; ++iteration)
  {
    apply_preconditioned_matrix(r, p);
    double a = -p.dot(r) / p.dot(p);
    x.add_a_times_x(a, r);
    r.add_a_times_x(a, p);
    if (monitor(r.norm(), iteration)) break;
  }

  // solve x = P\y
  if (d_P && d_pc_side == RIGHT)
  {
    d_z->copy(x);
    d_P->apply(*d_z, x);
  }

}

//----------------------------------------------------------------------------//
void MR1::apply_preconditioned_matrix(const Vector &v_in, Vector &v_out)
{
  if (d_P && d_pc_side == RIGHT)
  {
    d_P->apply(v_in, *d_z);
    d_A->multiply(*d_z, v_out);
  }
  else if (d_P && d_pc_side == LEFT)
  {
    d_A->multiply(v_in, *d_z);
    d_P->apply(*d_z, v_out);
  }
  else
  {
    d_A->multiply(v_in, v_out);
  }
}

} // end namespace callow

//----------------------------------------------------------------------------//
//              end of file MR1.cc
//----------------------------------------------------------------------------//

