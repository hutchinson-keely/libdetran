//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   Preconditioner.i
 *  @author Jeremy Roberts
 *  @brief  Python interface for callow preconditioners
 */
//----------------------------------------------------------------------------//

%shared_ptr(callow::Preconditioner)
%shared_ptr(callow::PCMatrix)
 
%include "Preconditioner.hh"
%include "PCMatrix.hh"