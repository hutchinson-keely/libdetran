//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  GeometryPlotter.cc
 *  @brief GeometryPlotter class definition
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#include "geometry/GeometryPlotter.hh"
#include "utilities/MathUtilities.hh"
#include <fstream>
#include <cstdio>
#include <sstream>
#ifdef DETRAN_ENABLE_OPENMP
#include <omp.h>
#endif

namespace detran_geometry
{

//----------------------------------------------------------------------------//
GeometryPlotter::GeometryPlotter(const std::string &prefix)
  : d_prefix(prefix)
  , d_dxyx(0.0)
  , d_nx(0)
  , d_ny(0)
{
  /* ... */
}

//----------------------------------------------------------------------------//
bool GeometryPlotter::initialize(SP_cartesianmesh mesh, const size_t n)
{
  Require(mesh);
  Require(n > 0);

  std::string filename = d_prefix;

  // physical extent
  double X = mesh->total_width_x();
  double Y = mesh->total_width_y();

  // minimum pixel size
  d_dxyx = 1e9;
  for (int d = 0; d < mesh->dimension(); ++d)
	  for (int i = 0; i < mesh->number_cells(d); ++i)
		  d_dxyx = std::min(d_dxyx, mesh->width(d, i));

  // resolution
  d_nx = n*std::ceil(X/d_dxyx);
  d_ny = n*std::ceil(Y/d_dxyx);
  d_x = detran_utilities::linspace(0, mesh->total_width_x(), d_nx);
  d_y = detran_utilities::linspace(0, mesh->total_width_y(), d_ny);
  d_cells.resize(d_nx * d_ny);
  for (size_t j = 0; j < d_ny; ++j)
  {
    for (size_t i = 0; i < d_nx; ++i)
    {
      size_t jj = j * d_nx;
      d_cells[i + jj] =
        mesh->find_cell(detran_geometry::Point(d_x[i], d_y[j]));
      if (d_cells[i + jj] < 0.0)
      {
        std::cout << " missing cell for x = " << d_x[i]
                  << " y = " << d_y[j] << std::endl;
      }
    }
  }

  // create plotter
  d_plotter = new detran_utilities::PPMPlotter;

  return true;
}

//----------------------------------------------------------------------------//
bool GeometryPlotter::initialize(SP_geometry geo, const double delta)
{
  Require(geo);
  Require(delta > 0.0);
  d_dxyx = delta;
  d_dxyx = std::min(d_dxyx, geo->width_x() / 5.0);
  d_dxyx = std::min(d_dxyx, geo->width_y() / 5.0);
  d_nx = std::max((int)std::ceil(geo->width_x()/d_dxyx), 5);
  d_ny = std::max((int)std::ceil(geo->width_y()/d_dxyx), 5);
  d_x = detran_utilities::linspace_center(0, geo->width_x(), d_nx);
  d_y = detran_utilities::linspace_center(0, geo->width_y(), d_ny);
  d_cells.resize(d_nx * d_ny);

  size_t i, j;
  #pragma omp parallel default(shared) private(i, j)
  {
    #pragma omp for
      for (j = 0; j < d_ny; ++j)
      {
        size_t jj = (d_ny - j - 1) * d_nx;
        for (i = 0; i < d_nx; ++i)
        {
          d_cells[i + jj] =
            geo->find_cell(detran_geometry::Point(d_x[i], d_y[j]));
          if (d_cells[i + jj] < 0.0)
          {
            std::cout << " missing cell for x = " << d_x[i]
                      << " y = " << d_y[j] << std::endl;
          }
        }
      }
  } // omp parallel

  // create plotter
  d_plotter = new detran_utilities::PPMPlotter;
  return true;
}

//----------------------------------------------------------------------------//
bool GeometryPlotter::write_mesh_map(SP_cartesianmesh   mesh,
                                     const std::string &key)
{
  Require(mesh);
  if (!mesh->mesh_map_exists(key))
  {
    std::cout << "Mesh map " + key + " doesn't exist; not writing mesh map"
              << std::endl;
    return false;
  }
  d_plotter->initialize(d_nx, d_ny, d_prefix+"_"+key+".ppm");
  vec_dbl data(d_cells.size());
  for (size_t i = 0; i < d_cells.size(); ++i)
  {
    Assert(d_cells[i] >= 0.0);
    data[i] = mesh->mesh_map(key)[d_cells[i]];
  }
  d_plotter->set_pixels<double>(data);
  return d_plotter->write();
}

//----------------------------------------------------------------------------//
bool GeometryPlotter::draw_geometry(SP_geometry  geo,
                                    bool         flag,
                                    const size_t cmap)
{
  Require(geo);

  d_plotter->initialize(d_nx, d_ny, d_prefix+"_geometry"+".ppm", cmap);
  vec_dbl data(d_cells.size());
  for (size_t i = 0; i < d_cells.size(); ++i)
  {
    data[i] = -1.0;
    if (d_cells[i] < 0) continue;
    if (flag)
      data[i] = (double) d_cells[i];
    else
      data[i] = (double) geo->region(d_cells[i])->attribute("MATERIAL");
  }
  d_plotter->set_pixels<double>(data);
  return d_plotter->write();

  return true;
}

//----------------------------------------------------------------------------//
//bool GeometryPlotter::write_scalar_flux(SP_mesh mesh, SP_state state)
//{
//  Require(mesh);
//  for (size_t g = 0; g < state->number_groups(); ++g)
//  {
//    std::cout << " g=" << g << std::endl;
//    std::stringstream g_;
//    g_ << g;
//    d_plotter->initialize(d_nx, d_ny,
//                          d_prefix+"_scalar_flux_"+g_.str()+".ppm");
//    vec_dbl data(d_cells.size());
//    for (size_t i = 0; i < d_cells.size(); ++i)
//    {
//      //Assert(d_cells[i] >= 0.0);
//      data[i] = state->phi(g)[d_cells[i]];
//    }
//    d_plotter->set_pixels<double>(data);
//    d_plotter->write();
//  }
//  return true;
//}

} // end namespace detran_geometry

//----------------------------------------------------------------------------//
//              end of file GeometryPlotter.cc
//----------------------------------------------------------------------------//

