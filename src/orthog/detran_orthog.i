//----------------------------------*-C++-*----------------------------------//
/**
 *  @file   detran_orthog.i
 *  @author Jeremy Roberts
 *  @brief  Python interface for detran orthogonal basis sets.
 */
//---------------------------------------------------------------------------//

%module(directors="1", allprotected="1", package="detran") orthog
%{
#include <stddef.h>
#include "orthog/OrthogonalBasisParameters.hh"
#include "orthog/OrthogonalBasis.hh"
#include "callow/detran_callow.hh"
%}

//%include "orthog/orthog_export.hh"
// Hide templates from SWIG
%inline
{
#define ORTHOG_EXPORT
#define ORTHOG_TEMPLATE_EXPORT(...)
#define ORTHOG_INSTANTIATE_EXPORT(...)
}

%import "utilities/detran_utilities.i"
%import "callow/detran_callow.i"

%shared_ptr(detran_orthog::OrthogonalBasis)
%shared_ptr(detran_orthog::OrthogonalBasisParameters)

%include "OrthogonalBasisParameters.hh"
%include "OrthogonalBasis.hh"

//---------------------------------------------------------------------------//
//              end of detran_orthog.i
//---------------------------------------------------------------------------//
