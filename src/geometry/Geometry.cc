//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  Geometry.cc
 *  @brief Geometry member definitions
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#include "geometry/Geometry.hh"

namespace detran_geometry
{

//----------------------------------------------------------------------------//
Geometry::Geometry(const double x, const double y, const double z)
  : Mesh(z == 0.0 ? 2 : 3)
  , d_x(x), d_y(y), d_z(z)
  , d_finalized(false)
{
  Require(d_x > 0);
  Require(d_y > 0);
  Require(d_z >= 0);
}

//----------------------------------------------------------------------------//
Geometry::SP_geometry
Geometry::Create(const double x, const double y, const double z)
{
  SP_geometry p(new Geometry(x, y, z));
  return p;
}

//----------------------------------------------------------------------------//
void Geometry::add_region(SP_region r)
{
  Require(r);
  d_finalized = false;
  d_regions.push_back(r);
}

//----------------------------------------------------------------------------//
void Geometry::finalize()
{
  Require(d_regions.size() > 0);
  d_number_cells = d_regions.size();

  // get attributes for first region
  const Region::attributes_t &attributes = d_regions[0]->attributes();

  // loop over all attributes, ensure all regions have it, and build full map
  for(Region::attributes_t::const_iterator iter = attributes.begin();
      iter != attributes.end();
      ++iter)
  {
    std::string key = iter->first;
    vec_int values(d_number_cells, iter->second);
    for (size_t r = 1; r < d_number_cells; ++r)
    {
      Assert(d_regions[r]->attribute_exists(key));
      values[r] = d_regions[r]->attribute(key);
    }
    add_mesh_map(key, values);
  }

  d_finalized = true;
}

//----------------------------------------------------------------------------//
Geometry::SP_region Geometry::region(const size_t r)
{
  Require(d_finalized);
  Require(r < number_cells());
  return d_regions[r];
}

//----------------------------------------------------------------------------//
int Geometry::find_cell(const Point &r) const
{
  int region = -1;
  for (size_t i = 0; i < number_cells(); i++)
  {
    if (d_regions[i]->contains(r))
    {
      region = i;
      break;
    }
  }
  return region;
}

//----------------------------------------------------------------------------//
void Geometry::display() const
{
  /* ... */
}

} // namespace detran_geometry

//----------------------------------------------------------------------------//
//              end of Geometry.cc
//----------------------------------------------------------------------------//

