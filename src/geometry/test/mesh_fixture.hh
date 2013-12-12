//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   mesh_fixture.hh
 *  @brief  CartesianMesh fixtures for testing
 *  @note   Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef MESH_FIXTURE_HH_
#define MESH_FIXTURE_HH_

#include "CartesianMesh.hh"
#include "PinCell.hh"
#include "Assembly.hh"
#include "Core.hh"
#include "Point.hh"
#include "DBC.hh"
#include "Definitions.hh"

namespace detran_test
{

using namespace detran_geometry;

/// Create a common 1D mesh for transport tests.
static CartesianMesh::SP_cartesianmesh mesh_1d_fixture()
{
  CartesianMesh::vec_dbl cm(3, 0.0);
  cm[1] = 5.0;
  cm[2] = 10.0;
  CartesianMesh::vec_int fm(2, 5);
  CartesianMesh::vec_int mat_map(2, 1);
  mat_map[0] = 0;
  CartesianMesh::SP_cartesianmesh mesh = Mesh1D::Create(fm, cm, mat_map);
  return mesh;
}

/// Create a common 2D mesh for transport tests.
static CartesianMesh::SP_cartesianmesh mesh_2d_fixture(int id = 0)
{
  CartesianMesh::SP_cartesianmesh mesh;

  if (id == 0)
  {
    // Define the coarse and fine meshes, and the coarse mesh material map.
    CartesianMesh::vec_dbl cm(3, 0.0);
    cm[1] = 10.0;
    cm[2] = 20.0;
    CartesianMesh::vec_int fm(2, 10);
    // All maps are stored in a single dimension.  The
    // memory order follows the transpose of the physical
    // order.  Here, the entries are (i=0,j=0), (i=0,j=1),
    // etc.  Hence, all entries are 1 but the first.
    CartesianMesh::vec_int mat_map(4, 1);
    mat_map[0] = 0;
    // Construct the mesh.
    mesh = Mesh2D::Create(fm, fm, cm, cm, mat_map);
  }
  else if (id == 1)
  {
    CartesianMesh::vec_dbl cm(2, 0.0);
    cm[1] = 1.0;
    CartesianMesh::vec_int fm(1, 3);
    CartesianMesh::vec_int mat_map(1, 1);
    // Construct the mesh.
    mesh = Mesh2D::Create(fm, fm, cm, cm, mat_map);

  }

  return mesh;
}

/// Create a common 3D mesh for transport tests.
static CartesianMesh::SP_cartesianmesh mesh_3d_fixture()
{
  CartesianMesh::vec_dbl cm(3, 0.0);
  cm[1] = 5.0;
  cm[2] = 10.0;
  CartesianMesh::vec_int fm(2, 5);
  CartesianMesh::vec_int mat_map(8, 1);
  mat_map[0] = 0;
  CartesianMesh::SP_cartesianmesh mesh =
    Mesh3D::Create(fm, fm, fm, cm, cm, cm, mat_map);
  return mesh;
}

// Defines a two region pin cell for use with material_fixture_2g
static PinCell::SP_pincell pincell_fixture()
{
  Point pitch(1.26, 1.26);
  detran_geometry::PinCell::vec_dbl radii(1, 0.54);
  detran_geometry::PinCell::vec_int mat_map(2, 0);
  mat_map[0] = 1;         // pin (fuel I)
  mat_map[1] = 0;         // mod
  bool fuel_flag = true;  // this is a fuel pin (i.e. not a moderator box)
  PinCell::SP_pincell pin(new PinCell(pitch, mat_map, radii));
  pin->meshify(7, true);
  return pin;
}

// Defines a 2x2 assembly for use with material_fixture_2g
static Assembly::SP_assembly assembly_fixture()
{
  Assembly::SP_assembly assembly(new detran_geometry::Assembly(2));
  Assembly::SP_pincell pin = pincell_fixture();
  assembly->add_pincell(pin);
  Assembly::vec_int pincell_map(4, 0);
  assembly->finalize(pincell_map);
  return assembly;
}

// Defines a 2x2 core for use with material_fixture_2g
static Core::SP_core core_fixture()
{
  Core::SP_core core(new detran_geometry::Core(2));
  Core::SP_assembly assembly = assembly_fixture();
  core->add_assembly(assembly);
  Core::vec_int assembly_map(4, 0);
  core->finalize(assembly_map);
  return core;
}

} // end namespace detran_test

#endif /* MESH_FIXTURE_HH_ */

//----------------------------------------------------------------------------//
//              end of mesh_fixture.hh
//----------------------------------------------------------------------------//
