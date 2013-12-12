//----------------------------------*-C++-*----------------------------------//
/**
 *  @file   detran_transport.i
 *  @author Jeremy Roberts
 *  @brief  Python interface for detran transport.
 */
//---------------------------------------------------------------------------//

%module(package="detran") transport
%{
#include <stddef.h>
#include "transport/DimensionTraits.hh"
#include "transport/FissionSource.hh"
#include "transport/State.hh"
#include "transport/SweepSource.hh"
#include "transport/Homogenize.hh"
#include "geometry/detran_geometry.hh"	
#include "external_source/detran_external_source.hh"
%}

// Hide templates from SWIG
%inline
{
#define TRANSPORT_EXPORT
#define TRANSPORT_TEMPLATE_EXPORT(...)
#define TRANSPORT_INSTANTIATE_EXPORT(...)
}

%import "detran_utilities.i"
%import "detran_material.i"
%import "detran_angle.i"
%import "detran_geometry.i"
%import "detran_external_source.i"
%import "detran_boundary.i"


%include "DimensionTraits.hh"

%include "State.hh"
%include "FissionSource.hh"
%include "ScatterSource.hh"
%template(StateSP)          detran_utilities::SP<detran::State>;
%template(FissionSourceSP)  detran_utilities::SP<detran::FissionSource>;
%template(ScatterSourceSP)  detran_utilities::SP<detran::ScatterSource>;

%include "Homogenize.hh"

//---------------------------------------------------------------------------//
//              end of detran_transport.i
//---------------------------------------------------------------------------//
