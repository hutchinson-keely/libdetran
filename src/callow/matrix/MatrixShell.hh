//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  MatrixShell.hh
 *  @brief MatrixShell class definition
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_MATRIXSHELL_HH_
#define callow_MATRIXSHELL_HH_

#include "MatrixBase.hh"

namespace callow
{

/**
 *  @class MatrixShell
 *  @brief Defines a matrix free operator
 *
 *  For many iterative methods, only the action of the operator
 *  on a vector is required.  Frequently, constructing a matrix
 *  explicitly is too memory intensive, and so a matrix free
 *  operator that defines the action is desirable.
 *
 *  This class is abstract. The client must define the action of
 *  the matrix and its transpose (though for the latter, the
 *  client may simply throw an exception to forbid its use).
 *
 */
class CALLOW_EXPORT MatrixShell: public MatrixBase
{

public:

  //--------------------------------------------------------------------------//
  // TYPEDEFS
  //--------------------------------------------------------------------------//

  typedef detran_utilities::SP<MatrixShell>    SP_matrix;

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  /// the context is the "this" of the caller
  MatrixShell(void* context);
  /// construct with known sizes
  MatrixShell(void* context, const int m, const int n);
  /// Virtual destructor
  virtual ~MatrixShell();

  //--------------------------------------------------------------------------//
  // PUBLIC FUNCTIONS
  //--------------------------------------------------------------------------//

  /// overloaded size setter so that the appropriate PETSc Mat is made
  void set_size(const int m, const int n = 0);

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL MATRICES MUST IMPLEMENT
  //--------------------------------------------------------------------------//

  // default shell assemble does nothing
  void assemble(){}
  // default shell display gives just the sizes
  void display(bool forceprint = false) const;
  // the client must implement the action y <-- A * x
  virtual void multiply(const Vector &x,  Vector &y) = 0;
  // the client must implement the action y <-- A' * x
  virtual void multiply_transpose(const Vector &x, Vector &y) = 0;

protected:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  /// context of my caller
  void* d_context;
  /// tell petsc about the operation, if applicable
  void set_operation();

};

#ifdef DETRAN_ENABLE_PETSC
// this is the function petsc actual calls; internally, it redirects
// to our own shell operation
inline PetscErrorCode shell_multiply_wrapper(Mat A, Vec x, Vec y);
#endif

CALLOW_TEMPLATE_EXPORT(detran_utilities::SP<MatrixShell>)

/**
 *  @class MatrixShellFunction
 *  @brief Expose the matrix shell to Python for user-defined action functions
 */
class MatrixShellFunction: public MatrixShell
{
public:

  /// Typedefs
  typedef void (*callback_ptr)(void *, Vector::SP_vector, Vector::SP_vector);

  MatrixShellFunction(const int m, const int n);

//  SP_pymatrix Create(const int m, const int n);

  // set the function that provides the action
  void set_multiply(callback_ptr f, void* data);
  // the client must implement the action y <-- A * x
  void multiply(const Vector &x, Vector &y);
  // the client must implement the action y <-- A' * x
  void multiply_transpose(const Vector &x, Vector &y);
private:
  callback_ptr d_multiply;
  callback_ptr d_multiply_transpose;

};

/**
 *  @class PyMatrixShell
 *  @brief Interface class for matrix shell for Python
 */
class PyMatrixShell: public MatrixShell
{
public:
  /// constructor
  PyMatrixShell(const int m, const int n);
  /// user must implement multiply
  virtual void multiply(Vector::SP_vector x, Vector::SP_vector y);
  /// user must implement multiply transpose
  virtual void multiply_transpose(Vector::SP_vector x, Vector::SP_vector y);
  /// overloaded multiply that calls the sp version
  void multiply(const Vector &x, Vector &y);
  /// overloaded multiply transpose that calls the sp version
  void multiply_transpose(const Vector &x, Vector &y);
};

} // end namespace callow

#include "MatrixShell.i.hh"

#endif /* callow_MATRIXSHELL_HH_ */

//----------------------------------------------------------------------------//
//              end of MatrixShell.hh
//----------------------------------------------------------------------------//
