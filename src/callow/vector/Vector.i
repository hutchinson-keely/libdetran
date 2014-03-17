//----------------------------------*-C++-*----------------------------------//
/**
 *  @file  Vector.i
 *  @brief Python interface for callow Vector
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//---------------------------------------------------------------------------//

%ignore *::operator[];

//
//%apply (double** ARGOUTVIEW_ARRAY1, int *DIM1) 
//       {(double** a, int *n)}
//%apply (int** ARGOUTVIEW_ARRAY1, int *DIM1) 
//       {(int** a, int *n)}
//%apply (double** ARGOUTVIEW_ARRAY2, int *DIM1,  int *DIM2) 
//       {(double** a, int *m, int *n)}

%shared_ptr(callow::Vector)
%shared_ptr(callow::Vector)
%include "Vector.hh"

%extend callow::Vector
{

  // get and set elements via, e.g., v0 = v[0], and v[0] = v0
  double  __getitem__(int i) 
  { 
    return (*self)[i]; 
  }
  void __setitem__(int i, double v) 
  { 
    (*self)[i] = v; 
  }
  
  // return the underlying data as a double array
  double *values()
  {
    return &(*self)[0];
  }
  
  %pythoncode 
  %{
    def array(self) :
        return
  %}
}

// Vector views.  These give us direct access to C arrays of 
// basic types via Numpy arrays.  We'll apply these below to 
// expose C++ std::vectors of those types.

%apply (double** ARGOUTVIEW_ARRAY1, int *DIM1) 
       {(double** a, int *n)}

%inline
{
  // Vectors to arrays (and then to Numpy). Careful, or you'll end up
  // in memory management pergatory.  These are used for plotting, etc.
  // Usage:
  //   v = Vector(10, 1.23) # or v = Vector.Create(10, 1.23)
  //   a = vec_asarray(v)
  //   a[0] = 2.34
  //   # do something in Numpy with a.  Then for good measure, do
  //   del a
  //   # and no matter what, don't use a after v is out of scope!
 
  void vec_asarray(callow::Vector &v, double **a, int *n)
  {
    *n = v.size();
    *a = &v[0];
  }
  
}