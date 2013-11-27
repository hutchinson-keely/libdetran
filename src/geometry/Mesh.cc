//----------------------------------*-C++-*----------------------------------//
/**
 *  @file   Mesh.hh
 *  @brief  Mesh class definition
 *  @note   Copyright (C) 2012-2013 Jeremy Roberts
 */
//---------------------------------------------------------------------------//

#include "geometry/Mesh.hh"
#include "utilities/DBC.hh"

namespace detran_geometry
{

//---------------------------------------------------------------------------//
Mesh::Mesh(const size_t d)
  : d_dimension(d)
  , d_number_cells(0)
{
  Insist(d > 0 && d <= 3, "Invalid dimension: " + AsString(d));
}

//---------------------------------------------------------------------------//
Mesh::~Mesh()
{
  /* ... */
}

//----------------------------------------------------------------------------//
Mesh::size_t Mesh::dimension() const
{
  return d_dimension;
}

//----------------------------------------------------------------------------//
Mesh::size_t Mesh::number_cells() const
{
  return d_number_cells;
}

//----------------------------------------------------------------------------//
void Mesh::add_mesh_map(const std::string &map_key, const vec_int &mesh_map)
{
  Require(!map_key.empty());
  Require(mesh_map.size() == d_number_cells);

  // Erase the value associated with the key if it exists.
  mesh_map_type::iterator it;
  it = d_mesh_map.find(map_key);
  if (it != d_mesh_map.end())
    d_mesh_map.erase(it);

  // Add the new value.
  d_mesh_map[map_key] = mesh_map;
}

//----------------------------------------------------------------------------//
bool Mesh::mesh_map_exists(const std::string &map_key)
{
  mesh_map_type::iterator iter;
  iter = d_mesh_map.find(map_key);
  if (iter != d_mesh_map.end())
    return true;
  else
    return false;
}

//----------------------------------------------------------------------------//
const Mesh::vec_int& Mesh::mesh_map(const std::string &map_key)
{
  Insist(mesh_map_exists(map_key), "Mesh map key not found:" + map_key);
  return d_mesh_map[map_key];
}

//----------------------------------------------------------------------------//
const Mesh::mesh_map_type& Mesh::get_mesh_map() const
{
  return d_mesh_map;
}

//----------------------------------------------------------------------------//
void Mesh::set_number_cells(const size_t n)
{
  Require(n > 0);
  d_number_cells = n;
  d_volume.resize(d_number_cells);
}

} // end namespace detran_geometry

//----------------------------------------------------------------------------//
//              end of Mesh.hh
//----------------------------------------------------------------------------//



