//----------------------------------*-C++-*----------------------------------//
/**
 *  @file   detran_callow.i
 *  @author Jeremy Roberts
 *  @brief  Python interface for callow library.
 */
//---------------------------------------------------------------------------//

%module(directors="1", allprotected="1", package="detran") callow
%{
#include <stddef.h>
#include "callow/callow_config.hh"
#include "callow/utils/Initialization.hh"
#include "callow/vector/Vector.hh"
#include "callow/matrix/MatrixBase.hh"
#include "callow/matrix/Matrix.hh"
#include "callow/matrix/MatrixShell.hh"
#include "callow/matrix/MatrixDense.hh"
#include "callow/solver/LinearSolver.hh"
#include "callow/solver/EigenSolver.hh"
#include "callow/preconditioner/Preconditioner.hh"
#include "callow/preconditioner/PCMatrix.hh"
%}

%feature("autodoc", "3");

// Hide templates from SWIG
%inline
{
#define CALLOW_EXPORT
#define CALLOW_TEMPLATE_EXPORT(...)
#define CALLOW_INSTANTIATE_EXPORT(...)
}

%import "detran_utilities.i"
 
//---------------------------------------------------------------------------//
// setup for numerical arrays
//---------------------------------------------------------------------------//

%{
#define SWIG_FILE_WITH_INIT
%}
%include "numpy.i"
%init %{
  import_array();
%}
%numpy_typemaps(double, NPY_DOUBLE, int)
%numpy_typemaps(in,     NPY_INT,    int)
 
//---------------------------------------------------------------------------//
// callow types
//---------------------------------------------------------------------------//

%include "utils/Initialization.hh"
%include "vector/Vector.i"
%include "matrix/Matrix.i"
%include "preconditioner/Preconditioner.i"
%include "solver/Solver.i"


%inline
{
  // std::vectors to arrays (and then to Numpy). Careful, or you'll end up
  // in memory management pergatory.  These are used for plotting, etc.
  //
  // Usage:
  //   v = vec_dbl(10, 1.23)
  //   a = vec_asarray(v)
  //   a[0] = 2.34
  //   # do something in Numpy with a.  Then for good measure, do
  //   del a
  //   # and no matter what, don't use a after v is out of scope!
  void vec_asarray(std::vector<double> &v, double **a, int *n)
  {
    *n = v.size();
    *a = &v[0];
  }
  void vec_asarray(std::vector<int> &v, int **a, int *n)
  {
    *n = v.size();
    *a = &v[0];
  }
}