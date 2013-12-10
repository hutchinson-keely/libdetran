//----------------------------------*-C++-*----------------------------------//
/**
 *  @file  Geometry.hh
 *  @brief Geometry class definition
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//---------------------------------------------------------------------------//

#ifndef detran_GEOMETRY_HH_
#define detran_GEOMETRY_HH_

#include "geometry/Mesh.hh"
#include "geometry/Region.hh"

namespace detran_geometry
{

/**
 *  @class Geometry
 *  @brief Represents a complete geometry comprised of @ref Region objects
 */
class GEOMETRY_EXPORT Geometry: public Mesh
{

public:

  //--------------------------------------------------------------------------//
  // TYPEDEFS
  //--------------------------------------------------------------------------//

  typedef detran_utilities::SP<Geometry>    SP_geometry;
  typedef Region::SP_region                 SP_region;
  typedef std::vector<SP_region>            vec_region;
  typedef detran_utilities::size_t          size_t;
  typedef detran_utilities::vec_size_t      vec_size_t;

  //--------------------------------------------------------------------------//
  // PUBLIC FUNCTIONS
  //--------------------------------------------------------------------------//

  /// Constructor
  Geometry(const double x, const double y, const double z = 0.0);

  /// SP constructor
  static SP_geometry
  Create(const double x, const double y, const double z = 0.0);

  /// Add a region
  void add_region(SP_region r);

  /// Finalize construction, which computes counts and region maps.
  void finalize();

  /// Get region
  SP_region region(const size_t r);

  /// Get geometry bounding box coordinates
  //@{
  double width_x() const {return d_x;}
  double width_y() const {return d_y;}
  double width_z() const {return d_z;}
  //@}


  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL MESHES MUST IMPLEMENT THESE
  //--------------------------------------------------------------------------//

  /// Find the cell containing a point
  int find_cell(const Point &p) const;

  /// Display some key features
  void display() const;

  //--------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL MESHES MUST IMPLEMENT THESE
  //--------------------------------------------------------------------------//

  /// Find the cell containing a point
 // int find_cell(const Point &p) const;

  /// Display some key features
//  void display() const;

private:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  //@{
  ///  Bounding box coordinates, where the origin is always at [0, 0, 0]
  double d_x;
  double d_y;
  double d_z;
  //@}

  /// Regions
  vec_region d_regions;

  /// Is it finalized?
  bool d_finalized;

};

} // end namespace detran_geometry

#endif /* detran_GEOMETRY_HH_ */

//----------------------------------------------------------------------------//
//              end of Geometry.hh
//----------------------------------------------------------------------------//
