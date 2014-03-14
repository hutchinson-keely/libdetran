//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  PCMatrix.hh
 *  @brief PCMatrix class definition
 *  @note  Copyright(C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_PCMatrix_HH_
#define callow_PCMatrix_HH_

#include "Preconditioner.hh"
#include "callow/matrix/MatrixBase.hh"

namespace callow
{

/**
 *  @class PCMatrix
 *  @brief Use the action of a user-defined matrix to precondition
 */

class CALLOW_EXPORT PCMatrix: public Preconditioner
{

public:

  //--------------------------------------------------------------------------//
  // TYPEDEFS
  //--------------------------------------------------------------------------//

  typedef MatrixBase::SP_matrix         SP_matrix;
  typedef Vector::SP_vector             SP_vector;

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  /// Construct a shell preconditioner
  PCMatrix(SP_matrix A, SP_db db = SP_db(0));

  /// Virtual destructor
  virtual ~PCMatrix(){};

  //--------------------------------------------------------------------------//
  // PUBLIC FUNCTIONS
  //--------------------------------------------------------------------------//

  /// Set a new preconditioner matrix
  void set_matrix(SP_matrix P_inv);

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL PRECONDITIONERS MUST IMPLEMENT THIS
  //--------------------------------------------------------------------------//

  /// Solve Px = b
  virtual void apply(Vector &b, Vector &x);

protected:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  /// User matrix
  SP_matrix d_P_inv;

};

} // end namespace callow

#endif /* callow_PCMatrix_HH_ */

//----------------------------------------------------------------------------//
//              end of PCMatrix.hh
//----------------------------------------------------------------------------//
