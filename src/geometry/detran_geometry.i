//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   detran_geometry.i
 *  @brief  Python interface for detran geometry
 *  @note   Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

%module(directors="1", allprotected="1", package="detran") geometry
%{
#include <stddef.h>
#include "utilities/SP.hh"
#include "geometry/Assembly.hh"
#include "geometry/Core.hh"
#include "geometry/Mesh.hh"
#include "geometry/CartesianMesh.hh"    
#include "geometry/PinCell.hh"
#include "geometry/Segment.hh"
#include "geometry/Track.hh"
#include "geometry/TrackDB.hh"
#include "geometry/Tracker.hh"
#include "geometry/Geometry.hh"
#include "geometry/Region.hh"
#include "geometry/CSG.hh"
#include "geometry/Point.hh"
#include "geometry/Ray.hh"
#include "geometry/QuadraticSurfaceFactory.hh"
#include "geometry/RegionFactory.hh"
// Fix for missing SWIGPY_SLICE_ARG with some versions of swig.
#if PY_VERSION_HEX >= 0x03020000
# define SWIGPY_SLICE_ARG(obj) ((PyObject*) (obj))
#else
# define SWIGPY_SLICE_ARG(obj) ((PySliceObject*) (obj))
#endif
%}

%feature("autodoc", "3");

%include "geometry/geometry_export.hh"

// Hide templates from SWIG
%inline
{
//#define GEOMETRY_EXPORT
//#define GEOMETRY_TEMPLATE_EXPORT(...)
//#define GEOMETRY_INSTANTIATE_EXPORT(...)
#define BOOST_CLASS_EXPORT_KEY(...)
}

%import "utilities/detran_utilities.i"
%import "angle/detran_angle.i"
//%include "std_vector.i"

%include "Point.hh"
%include "Ray.hh"

%shared_ptr(detran_geometry::Mesh)
%shared_ptr(detran_geometry::CartesianMesh)
%include "Mesh.hh"
%include "CartesianMesh.hh"

%shared_ptr(detran_geometry::PinCell)
%shared_ptr(detran_geometry::Assembly)
%shared_ptr(detran_geometry::Core)
%include "PinCell.hh"
%include "Assembly.hh"
%include "Core.hh"

// CSG 
%shared_ptr(detran_geometry::Surface)
%shared_ptr(detran_geometry::QuadraticSurface)
%shared_ptr(detran_geometry::Region)
%shared_ptr(detran_geometry::Geometry)
%include "Surface.hh"
%include "QuadraticSurface.hh"
%include "QuadraticSurfaceFactory.hh"
%include "CSG.hh"
%include "Region.hh"
%include "Geometry.hh"
%include "RegionFactory.hh"

// MOC
%shared_ptr(detran_geometry::Track)
%shared_ptr(detran_geometry::TrackDB)
%shared_ptr(detran_geometry::Tracker)
%include "Segment.hh"
%include "Track.hh"
%include "TrackDB.hh"
%include "Tracker.hh"

%template(vec_region) std::vector<detran_utilities::SP<detran_geometry::Region> >;

//----------------------------------------------------------------------------//
//              end of detran_geometry.i
//----------------------------------------------------------------------------//
