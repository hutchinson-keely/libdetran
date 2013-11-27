//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  Mesh.i.hh
 *  @brief Mesh inline member definitions
 *  @note  Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef detran_geometry_MESH_I_HH_
#define detran_geometry_MESH_I_HH_

namespace detran_geometry
{

//----------------------------------------------------------------------------//
inline CartesianMesh::size_t CartesianMesh::number_cells(size_t dim) const
{
  if (dim == 0)
    return d_number_cells_x;
  else if (dim == 1)
    return d_number_cells_y;
  else
    return d_number_cells_z;
}

//----------------------------------------------------------------------------//
inline CartesianMesh::size_t CartesianMesh::number_cells_x() const
{
  return d_number_cells_x;
}
inline CartesianMesh::size_t CartesianMesh::number_cells_y() const
{
  return d_number_cells_y;
}
inline CartesianMesh::size_t CartesianMesh::number_cells_z() const
{
  return d_number_cells_z;
}

//----------------------------------------------------------------------------//

inline double CartesianMesh::width(size_t dim, size_t ijk) const
{
  Require(dim <  3);
  if (dim == 0)
    return dx(ijk);
  else if (dim == 1)
    return dy(ijk);
  else
    return dz(ijk);
}
inline double CartesianMesh::dx(size_t i) const
{
  Require (i < d_number_cells_x);
  return d_dx[i];
}
inline double CartesianMesh::dy(size_t j) const
{
  Require (j < d_number_cells_y);
  return d_dy[j];
}
inline double CartesianMesh::dz(size_t k) const
{
  Require (k < d_number_cells_z);
  return d_dz[k];
}

//----------------------------------------------------------------------------//
inline double CartesianMesh::volume(const size_t cell) const
{
  Require(cell < d_number_cells);
  double v = dx(cell_to_i(cell)) *
             dy(cell_to_j(cell)) *
             dz(cell_to_k(cell));
  Ensure(v > 0.0);
  return v;
}

//----------------------------------------------------------------------------//
inline double CartesianMesh::total_width_x() const
{
  return d_total_width_x;
}
inline double CartesianMesh::total_width_y() const
{
  return d_total_width_y;
}
inline double CartesianMesh::total_width_z() const
{
  return d_total_width_z;
}

//----------------------------------------------------------------------------//
inline CartesianMesh::size_t
CartesianMesh::index(const size_t i, const size_t j, const size_t k) const
{
  Require(i >= 0);
  Require(i < d_number_cells_x);
  Require(j >= 0);
  Require(j < d_number_cells_y);
  Require(k >= 0);
  Require(k < d_number_cells_z);
  return i + j * d_number_cells_x + k * d_number_cells_x * d_number_cells_y;
}

//----------------------------------------------------------------------------//
inline CartesianMesh::size_t CartesianMesh::cell_to_i(const size_t cell) const
{
  Require(cell >= 0);
  Require(cell < d_number_cells);
  size_t i = cell % d_number_cells_x;
  Ensure(i < d_number_cells_x);
  return i;
}
inline CartesianMesh::size_t CartesianMesh::cell_to_j(const size_t cell) const
{
  Require(cell >= 0);
  Require(cell < d_number_cells);
  size_t j = cell % (d_number_cells_x * d_number_cells_y);
  double tmp = std::floor(double(j)/double(d_number_cells_x));
  j = size_t(tmp);
  Ensure(j < d_number_cells_y);
  return j;
}
inline CartesianMesh::size_t CartesianMesh::cell_to_k(const size_t cell) const
{
  Require(cell >= 0);
  Require(cell < d_number_cells);
  double tmp = std::floor(double(cell)/double(d_number_cells_x*d_number_cells_y));
  size_t k = size_t(tmp);
  Ensure(k < d_number_cells_z);
  return k;
}
inline CartesianMesh::size_t
CartesianMesh::cell_to_ijk(const size_t dim, const size_t cell) const
{
  Require(cell >= 0);
  Require(cell < d_number_cells);
  Require(dim >= 0 && dim < dimension());
  if (dim == 0)
    return cell_to_i(cell);
  else if (dim == 1)
    return cell_to_j(cell);
  else
    return cell_to_k(cell);
}

//----------------------------------------------------------------------------//
inline int CartesianMesh::find_cell(const Point &p) const
{
  // Default is -1, meaning not found
  int cell;
  if ( (p.x() < 0.0) || (p.x() > d_total_width_x) ||
       (p.y() < 0.0) || (p.y() > d_total_width_y) ||
       (p.z() < 0.0) || (p.z() > d_total_width_z) )
  {
    cell = -1;
  }
  else
  {
    cell = 0;
    bool found = false;
    size_t ijk[] = {0, 0, 0};
    double xyz[] = {p.x(), p.y(), p.z()};
    for (size_t d = 0; d < dimension(); ++d)
    {
      double current_edge = 0.0;
      for (size_t i = 0; i < number_cells(d); ++i)
      {
        current_edge += width(d, i);
        if (xyz[d] <= current_edge)
        {
          ijk[d] = i;
          found = true;
          break;
        }
      }
    }
    Ensure(found);
    cell = index(ijk[0], ijk[1], ijk[2]);
  }
  return cell;
}

} // end namespace detran

#endif // MESH_I_HH_ 

//----------------------------------------------------------------------------//
//              end of file Mesh.i.hh
//----------------------------------------------------------------------------//
