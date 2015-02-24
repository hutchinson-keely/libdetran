//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  GeometryPlotter.hh
 *  @brief GeometryPlotter class definition
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef detran_geometry_GeometryPlotter_HH_
#define detran_geometry_GeometryPlotter_HH_

#include "geometry/geometry_export.hh"
#include "geometry/CartesianMesh.hh"
#include "geometry/Geometry.hh"
#include "utilities/PPMPlotter.hh"

namespace detran_geometry
{

/**
 *  @class GeometryPlotter
 *  @brief Produces 2-D plots of the mesh
 *
 *  This is really limited, as no color bar or other information is
 *  displayed---just colors.  It's mostly as a fast way to plot the
 *  mesh when Python+matplotlib isn't the answer.
 */
class GEOMETRY_EXPORT GeometryPlotter
{

public:

  //--------------------------------------------------------------------------//
  // TYPEDEFS
  //--------------------------------------------------------------------------//

  typedef detran_utilities::PPMPlotter::SP_ppmplotter  	SP_ppmplotter;
  typedef detran_utilities::size_t                		size_t;
  typedef detran_utilities::vec_dbl               		vec_dbl;
  typedef detran_utilities::vec_int               		vec_int;
  typedef CartesianMesh::SP_cartesianmesh			    SP_cartesianmesh;
  typedef Geometry::SP_geometry  						SP_geometry;

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR AND DESTRUCTOR
  //--------------------------------------------------------------------------//

  GeometryPlotter(const std::string &prefix = "detran");

  //--------------------------------------------------------------------------//
  // PUBLIC FUNCTIONS
  //--------------------------------------------------------------------------//

  /// Initialize file for writing Cartesian mesh data
  bool initialize(SP_cartesianmesh mesh, const size_t n = 1);

  /// Initialize file for writing geometry data
  bool initialize(SP_geometry geo, const double delta = 0.1);

  /**
   *  @brief Write a mesh map to file.
   *  @param key  Key of the map to write
   *  @return     True for successful write
   */
  bool write_mesh_map(SP_cartesianmesh mesh, const std::string &key);

  /**
   *  @brief Write the multigroup scalar flux moments to file.
   *  @param state  State vector container
   *  @return     True for successful write
   */
  //bool write_scalar_flux(SP_cartesianmesh mesh, SP_state state);

  /// Draw a geometry (in the xy plane) using region index or material
  bool draw_geometry(SP_geometry geo, bool flag, const size_t cmap = 0);

private:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  /// Filename prefix
  std::string d_prefix;
  /// Plotter
  SP_ppmplotter d_plotter;
  /// Mesh
  SP_cartesianmesh d_mesh;
  /// Mesh resolution
  double d_dxyx;
  /// X resolution
  size_t d_nx;
  /// Y resolution
  size_t d_ny;
  /// x values
  vec_dbl d_x;
  /// y values
  vec_dbl d_y;
  /// cells for x and y
  vec_int d_cells;

};

} // end namespace detran_geometry

#endif /* detran_geometry_GeometryPlotter_HH_ */

//----------------------------------------------------------------------------//
//              end of file GeometryPlotter.hh
//----------------------------------------------------------------------------//
