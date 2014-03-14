//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  PCILU0.hh
 *  @brief PCILU0 class definition
 *  @note  Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_PCILU0_HH_
#define callow_PCILU0_HH_

#include "Preconditioner.hh"
#include "callow/matrix/Matrix.hh"

namespace callow
{

/**
 *  @class PCILU0
 *  @brief Implements the ILU(0) preconditioner
 *
 *  Following Saad, ILU(0) is defined
 *  @code
 *    for i = 2, n
 *      for k = 1, i - 1
 *        for (i, k) in nonzeros of lower A
 *          A(i,k) = A(i,k)/A(k,k)
 *          for j = k + 1 .. n
 *            for (i, j) in nonzeros of upper A
 *              A(i, j) = A(i, j) - A(i, k)*A(k, k)
 *            end
 *          end
 *        end
 *      end
 *    end
 *  @endcode
 */

class CALLOW_EXPORT PCILU0: public Preconditioner
{

public:

  //--------------------------------------------------------------------------//
  // TYPEDEFS
  //--------------------------------------------------------------------------//

  typedef Matrix::SP_matrix                 SP_matrixfull;

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  PCILU0(SP_matrix A, SP_db db = SP_db(0));

  virtual ~PCILU0(){};

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL PRECONDITIONERS MUST IMPLEMENT THIS
  //--------------------------------------------------------------------------//

  void apply(Vector &b, Vector &x);

protected:

  /// ILU decomposition of A
  SP_matrixfull d_P;
  /// Working vector
  Vector d_y;

};

} // end namespace callow

#endif // callow_PCILU0_HH_

//----------------------------------------------------------------------------//
//              end of file PCILU0.hh
//----------------------------------------------------------------------------//
