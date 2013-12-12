//----------------------------------*-C++-*----------------------------------//
/**
 *  @file   Mesh.hh
 *  @brief  Mesh class definition
 *  @note   Copyright (C) 2012-2013 Jeremy Roberts
 */
//---------------------------------------------------------------------------//

#ifndef detran_geometry_MESH_HH_
#define detran_geometry_MESH_HH_

#include "geometry/geometry_export.hh"
#include "geometry/Point.hh"
#include "utilities/Definitions.hh"
#include "utilities/SP.hh"
#include <map>

namespace detran_geometry
{

/**
 *  @class Mesh
 *  @brief Abstract mesh class for structured and unstructured meshes
 */
class GEOMETRY_EXPORT Mesh
{

public:

  //--------------------------------------------------------------------------//
  // ENUMERATIONS
  //--------------------------------------------------------------------------//

  enum SIDES
  {
    WEST, EAST, SOUTH, NORTH, BOTTOM, TOP, END_SIDES
  };

  enum FACE2D
  {
    VERT, HORZ
  };

  enum FACE3D
  {
    YZ, XZ, XY
  };

  //--------------------------------------------------------------------------//
  // TYPEDEFS
  //--------------------------------------------------------------------------//

  typedef detran_utilities::SP<Mesh>        SP_mesh;
  typedef detran_utilities::vec_int         vec_int;
  typedef detran_utilities::vec_dbl         vec_dbl;
  typedef detran_utilities::size_t          size_t;
  typedef std::map<std::string, vec_int>    mesh_map_type;

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  Mesh(const size_t d);

  virtual ~Mesh();

  //--------------------------------------------------------------------------//
  // PUBLIC FUNCTIONS
  //--------------------------------------------------------------------------//

  /// Get the mesh dimension
  size_t dimension() const;

  /// Get total number of cells
  size_t number_cells() const;

  /// Get cell volume
  virtual double volume(const size_t cell) const;

  /**
   * @brief  Add map of fine mesh integer properties.
   *
   * If the key exists, this function overwrites the map.
   *
   * @param  map_key   String description of map.
   * @param  mesh_map  Logically multi-dimensional map as 1-d vector.
   */
  void add_mesh_map(const std::string &map_key, const vec_int &mesh_map);

  /// Check if fine mesh map exists.
  bool mesh_map_exists(const std::string &map_key);

  /// Get a mesh map
  const vec_int& mesh_map(const std::string &map_key);

  /// Return a const reference to the full map (useful for IO)
  const mesh_map_type& get_mesh_map() const;

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL MESHES MUST IMPLEMENT THESE
  //--------------------------------------------------------------------------//

  /// Find the cell containing a point.  Returns negative if not found.
  virtual int find_cell(const Point &p) const = 0;

  /// Display mesh information
  virtual void display() const = 0;

protected:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  /// Dimension
  size_t d_dimension;

  /// Number of spatial cells
  size_t d_number_cells;

  /// Cell volumes
  vec_dbl d_volume;

  /**
   *  Map container containing a key describing a mesh property and a fine
   *  mesh map defining the property in each cell.  These properties
   *  include materials, coarse mesh regions (pins, assembly, fuel,
   *  moderator, etc.), and anything else the user wants to edit.
   */
  mesh_map_type d_mesh_map;

  //--------------------------------------------------------------------------//
  // IMPLEMENTATION
  //--------------------------------------------------------------------------//

  Mesh() : d_dimension(0), d_number_cells(0) {}

  void set_number_cells(const size_t);

};

GEOMETRY_TEMPLATE_EXPORT(detran_utilities::SP<Mesh>)

} // end namespace detran_geometry

#endif /* detran_geometry_MESH_HH_ */

//----------------------------------------------------------------------------//
//              end of Mesh.hh
//----------------------------------------------------------------------------//
