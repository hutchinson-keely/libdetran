//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  Matrix.i
 *  @brief Python interface for callow Matrix
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

%ignore *::operator[];

//----------------------------------------------------------------------------//
// Extensions for callow::Matrix
//----------------------------------------------------------------------------//

%apply (int*    IN_ARRAY1, int DIM1) {(int*    i, int dim_i)}
%apply (int*    IN_ARRAY1, int DIM1) {(int*    j, int dim_j)}
%apply (double* IN_ARRAY1, int DIM1) {(double* v, int dim_v)}
%apply (int**    ARGOUTVIEW_ARRAY1, int *DIM1){(int**    v, int *dim_v)}
%apply (double** ARGOUTVIEW_ARRAY1, int *DIM1){(double** v, int *dim_v)}

%rename (insert) callow::Matrix::py_insert;
%extend callow::Matrix 
{
  // bool insert(int  i, int *j, double *v, int n, const int type = INSERT);
  bool py_insert(int i, int *j, int dim_j, double *v, int dim_v, 
                 const int type = INSERT) 
  {
    if (dim_j != dim_v) 
    {
      PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d,%d) given",
                   dim_j, dim_v);
      return false;
    }
    return self->insert(i, j, v, dim_v, type);
  }
  // bool insert(int *i, int  j, double *v, int n, const int type = INSERT);
  bool py_insert(int *i, int dim_i, int j, double *v, int dim_v, 
                 const int type = INSERT) 
  {
    if (dim_i != dim_v) 
    {
      PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d,%d) given",
                   dim_i, dim_v);
      return false;
    }
    return self->insert(i, j, v, dim_v, type);
  }
  // bool insert(int *i, int  j, double *v, int n, const int type = INSERT);
  bool py_insert(int *i, int dim_i, int *j, int dim_j, double *v, int dim_v, 
                 const int type = INSERT) 
  {
    if (dim_i != dim_v) 
    {
      PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d,%d) given",
                   dim_i, dim_v);
      return false;
    }
    if (dim_j != dim_v)
    {
      PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d,%d) given",
                   dim_j, dim_v);
      return false;
    }
    return self->insert(i, j, v, dim_v, type);
  }
  double  __getitem__(int i, int j) 
  { 
    return (*self)(i, j);
  }
  // get csr arrays
  void get_values(double **v, int *dim_v)
  {
    *v     = self->values();
    *dim_v = self->number_nonzeros();
  }
  void get_columns(int **v, int *dim_v)
  {
    *v     = self->columns();
    *dim_v = self->number_nonzeros();
  }
  void get_rows(int **v, int *dim_v)
  {
    *v     = self->rows();
    *dim_v = self->number_rows() + 1;
  }
};
%exception Matrix::py_insert
{
  $action
  if (PyErr_Occurred()) SWIG_fail;
}


//----------------------------------------------------------------------------//
// Extensions for callow::MatrixDense
//----------------------------------------------------------------------------//
 
%apply (int*    IN_ARRAY1, int DIM1) {(int*    i, int dim_i)}
%apply (int*    IN_ARRAY1, int DIM1) {(int*    j, int dim_j)}
%apply (double* IN_ARRAY1, int DIM1) {(double* v, int dim_v)}

%rename (insert)     callow::MatrixDense::py_insert;
%rename (insert_row) callow::MatrixDense::py_insert_row;
%rename (insert_col) callow::MatrixDense::py_insert_col;

%extend callow::MatrixDense
{
  // bool insert(double *v, const int type = INSERT);
  bool py_insert(double* v, int dim_v, const int type = INSERT) 
  {
    int n = self->number_rows()*self->number_columns();
    if (dim_v != n) 
    {
      PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d,%d) given",
                   dim_v, n);
      return false;
    }
    return self->insert(v, type);
  }
  // bool insert_row(int  i, double *v, const int type = INSERT);
  bool py_insert_row(int i, double *v, int dim_v, const int type = INSERT) 
  {
    int n = self->number_columns();
    if (dim_v != n) 
    {
      PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d,%d) given",
                   dim_v, n);
      return false;
    }
    return self->insert_row(i, v, type);
  }
  // bool insert_col(int  j, double *v, const int type = INSERT);
  bool py_insert_col(int j, double *v, int dim_v, const int type = INSERT) 
  {
    int n = self->number_rows();
    if (dim_v != n) 
    {
      PyErr_Format(PyExc_ValueError, "Arrays of lengths (%d,%d) given",
                   dim_v, n);
      return false;
    }
    return self->insert_col(j, v, type);
  }
  double  __getitem__(int i, int j) 
  { 
    return (*self)(i, j);
  }
};
%exception MatrixDense::py_insert
{
  $action
  if (PyErr_Occurred()) SWIG_fail;
}
%exception MatrixDense::py_insert_row
{
  $action
  if (PyErr_Occurred()) SWIG_fail;
}
%exception MatrixDense::py_insert_col
{
  $action
  if (PyErr_Occurred()) SWIG_fail;
}

//----------------------------------------------------------------------------//
// Extensions for shell matrices
//----------------------------------------------------------------------------//

%feature("director") callow::MatrixShell;
%feature("director") callow::PyShell;
%feature("nodirector") callow::PyShell::multiply(const callow::Vector &x, callow::Vector &y);

%feature("director:except") 
{
    if( $error != NULL ) 
    {
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch( &ptype, &pvalue, &ptraceback );
        PyErr_Restore( ptype, pvalue, ptraceback );
        PyErr_Print();
        Py_Exit(1);
    }
} 
%exception {
    try { $action }
    catch (Swig::DirectorException &e) { SWIG_fail; }
}

%include callback.i 

// Set the callback setter for python-based time-dependent materials
setCallbackMethod(1, // this is a *unique* identifier
                  callow::PyMatrixShell, 
                  set_multiply, 
                  (void* data, callow::Vector::SP_vector x, callow::Vector::SP_vector y), 
                  (callow::Vector::SP_vector x, callow::Vector::SP_vector y), 
                  (x, y), 1)

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

%shared_ptr(callow::MatrixBase)
%shared_ptr(callow::Matrix)
%shared_ptr(callow::MatrixDense)
%shared_ptr(callow::MatrixShell)
%shared_ptr(callow::PyMatrixShell)
%shared_ptr(callow::PyShell)
%include "MatrixBase.hh"
%include "Matrix.hh"
%include "MatrixDense.hh"
%include "MatrixShell.hh"


%apply (double** ARGOUTVIEW_ARRAY2, int* DIM1, int* DIM2)
       {(double** a, int* m, int* n)}

%inline
{
  // View dense matrix as numpy matrix
  void mat_asarray(callow::MatrixDense &M, double **a, int *m, int *n)
  {
    *m = M.number_rows();
    *n = M.number_columns();
    *a = &M[0];
  }
}

