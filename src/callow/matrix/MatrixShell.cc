//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  MatrixShell.cc
 *  @brief MatrixShell member definitions
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#include "MatrixShell.hh"

namespace callow
{

//----------------------------------------------------------------------------//
MatrixShell::MatrixShell(void* context)
  : d_context(context)
{
  /* ... */
}

//----------------------------------------------------------------------------//
MatrixShell::MatrixShell(void* context, const int m, const int n)
  : d_context(context)
{
  set_size(m, n);
}

//----------------------------------------------------------------------------//
MatrixShell::~MatrixShell()
{
  /* ... */
}

//----------------------------------------------------------------------------//
void MatrixShell::set_size(const int m, const int n)
{
  Require(m > 0);
  d_m = m;
  d_n = n;
  if (!d_n) d_n = d_m;
  d_sizes_set = true;
#ifdef DETRAN_ENABLE_PETSC
  PetscErrorCode ierr;
  ierr = MatCreate(PETSC_COMM_SELF, &d_petsc_matrix);
  ierr = MatSetSizes(d_petsc_matrix, d_m, d_n, PETSC_DETERMINE, PETSC_DETERMINE);
  ierr = MatSetType(d_petsc_matrix, MATSHELL);
  ierr = MatShellSetContext(d_petsc_matrix, d_context);
  ierr = MatSetUp(d_petsc_matrix);
  Ensure(!ierr);
#endif
  set_operation();
  d_is_ready = true;
}

//----------------------------------------------------------------------------//
void MatrixShell::display(bool forceprint) const
{
  std::cout << "MatrixShell:" << std::endl
            << "  # rows = " << d_m << std::endl
            << "  # cols = " << d_n << std::endl
            << std::endl;
}

//----------------------------------------------------------------------------//
MatrixShellFunction::MatrixShellFunction(const int m, const int n)
  : MatrixShell(NULL, m, n)
  , d_multiply(NULL)
  , d_multiply_transpose(NULL)
{
  /* ... */
}

//----------------------------------------------------------------------------//
void MatrixShellFunction::set_multiply(callback_ptr f, void* data)
{
  d_multiply = f;
  d_context  = data;
}

//----------------------------------------------------------------------------//
void MatrixShellFunction::multiply(const Vector &x, Vector &y)
{
  SP_vector X(new Vector(x.size(), const_cast<double*>(&x[0])));
  SP_vector Y(new Vector(y.size(), &y[0]));
  d_multiply(d_context, X, Y);
}

//----------------------------------------------------------------------------//
void MatrixShellFunction::multiply_transpose(const Vector &x, Vector &y)
{
  THROW("NOT IMPLEMENTED");
}


//MatrixShellFunction::SP_pymatrix
//MatrixShellFunction::Create(const int m, const int n)
//{
//  detran_utilities::SP<PyMatrixShell> p(new PyMatrixShell(m, n));
//  return p;
//}

//----------------------------------------------------------------------------//
PyMatrixShell::PyMatrixShell(const int m, const int n)
  : MatrixShell(NULL, m, n)
{
  /* ... */
}


//----------------------------------------------------------------------------//
void PyMatrixShell::multiply(Vector::SP_vector x, Vector::SP_vector y)
{
  THROW("NOT IMPLEMENTED");
}

//----------------------------------------------------------------------------//
void PyMatrixShell::multiply_transpose(Vector::SP_vector x, Vector::SP_vector y)
{
  THROW("NOT IMPLEMENTED");
}

//----------------------------------------------------------------------------//
void PyMatrixShell::multiply(const Vector &x, Vector &y)
{
  SP_vector  X(new Vector(x.size(), const_cast<double*>(&x[0])));
  SP_vector  Y(new Vector(y.size(), &y[0]));
  multiply(X, Y);
}

//----------------------------------------------------------------------------//
void PyMatrixShell::multiply_transpose(const Vector &x, Vector &y)
{
  SP_vector  X(new Vector(x.size(), const_cast<double*>(&x[0])));
  SP_vector  Y(new Vector(y.size(), &y[0]));
  multiply_transpose(X, Y);
}


} // end namespace callow

//----------------------------------------------------------------------------//
//              end of MatrixShell.cc
//----------------------------------------------------------------------------//
