//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  PCMatrix.cc
 *  @brief PCMatrix member definitions
 *  @note  Copyright(C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#include "PCMatrix.hh"

namespace callow
{

//----------------------------------------------------------------------------//
PCMatrix::PCMatrix(SP_matrix P_Inv, std::string name)
  : Preconditioner(name)
  , d_P_inv(P_Inv)
{
  if (d_P_inv) d_size = d_P_inv->number_columns();
}

//----------------------------------------------------------------------------//
void PCMatrix::set_matrix(SP_matrix P_inv)
{
  Require(P_inv);
  d_P_inv = P_inv;
  d_size = d_P_inv->number_columns();
}

//----------------------------------------------------------------------------//
void PCMatrix::apply(Vector &b, Vector &x)
{
  Require(d_P_inv);
  d_P_inv->multiply(b, x);
}

} // end namespace callow

//----------------------------------------------------------------------------//
//              end of PCMatrix.cc
//----------------------------------------------------------------------------//
