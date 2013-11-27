//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   test_Mesh1D.cc
 *  @brief  Test of CartesianMesh and 1/2/3d helpers
 *  @note   Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

// LIST OF TEST FUNCTIONS
#define TEST_LIST                   \
        FUNC(test_Mesh1D)           \
        FUNC(test_Mesh2D)           \
        FUNC(test_Mesh3D)           \
        FUNC(test_Mesh1D_serialize)

// Detran headers
#include "TestDriver.hh"
#include "CartesianMesh.hh"

// System headers
#include <fstream>

// Setup
#include "mesh_fixture.hh"

using namespace detran_geometry;
using namespace detran_utilities;
using namespace detran_test;
using namespace std;

int main(int argc, char *argv[])
{
  RUN(argc, argv);
}

//----------------------------------------------------------------------------//
// TEST DEFINITIONS
//----------------------------------------------------------------------------//

typedef CartesianMesh::SP_cartesianmesh SP_mesh;

//----------------------------------------------------------------------------//
int test_Mesh1D(int argc, char *argv[])
{
  // Get the mesh
  SP_mesh mesh = mesh_1d_fixture();

  // Mesh properties:
  //   cm = [ 0.0  5.0 10.0]
  //   fm = [    5    5    ]
  //   mt = [    0    1    ]

  // Basic mesh things.
  TEST(mesh->number_cells()     == 10);
  TEST(mesh->number_cells_x()   == 10);
  TEST(mesh->number_cells_y()   == 1);
  TEST(mesh->number_cells_z()   == 1);
  TEST(mesh->dx(0)              == 1.0);
  TEST(mesh->dy(0)              == 1.0);
  TEST(mesh->dz(0)              == 1.0);
  TEST(mesh->dimension()        == 1);
  int cell = 5;
  TEST(mesh->index(5)           == cell);
  TEST(mesh->cell_to_i(cell)    == 5);
  TEST(mesh->cell_to_j(cell)    == 0);
  TEST(mesh->cell_to_k(cell)    == 0);
  TEST(mesh->volume(cell)       == 1.0);

  vec_int mat_map = mesh->mesh_map("MATERIAL");
  TEST(mat_map[0]               == 0);
  TEST(mat_map[5]               == 1);
  TESTFALSE(mesh->mesh_map_exists("SHOULDNOTEXIST"));

  return 0;
}

//----------------------------------------------------------------------------//
int test_Mesh2D(int argc, char *argv[])
{
  // Get the mesh
  SP_mesh mesh = mesh_2d_fixture();

  // Basic mesh things.
  TEST(mesh->number_cells()     == 400);
  TEST(mesh->number_cells_x()   == 20);
  TEST(mesh->number_cells_y()   == 20);
  TEST(mesh->number_cells_z()   == 1);
  TEST(mesh->dx(0)              == 1.0);
  TEST(mesh->dy(0)              == 1.0);
  TEST(mesh->dz(0)              == 1.0);
  TEST(mesh->dimension()        == 2);
  int cell = 5 + 5*20;
  TEST(mesh->index(5, 5)        == cell);
  TEST(mesh->cell_to_i(cell)    == 5);
  TEST(mesh->cell_to_j(cell)    == 5);

  vec_int mat_map = mesh->mesh_map("MATERIAL");
  TEST(mat_map[0]               == 0);
  TEST(mat_map[10]              == 1);

  return 0;
}

//----------------------------------------------------------------------------//
int test_Mesh3D(int argc, char *argv[])
{
  // Get the mesh
  SP_mesh mesh = mesh_3d_fixture();

  // Mesh properties:
  //   cm = [ 0.0  5.0 10.0]
  //   fm = [    5    5   ]
  //   mt = [ 0 1; 1 1; 1 1; 1 1]

  // Basic mesh things.
  TEST(mesh->number_cells()     == 1000);
  TEST(mesh->number_cells_x()   == 10);
  TEST(mesh->number_cells_y()   == 10);
  TEST(mesh->number_cells_z()   == 10);
  TEST(mesh->dx(0)              == 1.0);
  TEST(mesh->dy(0)              == 1.0);
  TEST(mesh->dz(0)              == 1.0);
  TEST(mesh->dimension()        == 3);
  int cell = 5 + 5*10 + 5*100;
  TEST(mesh->index(5, 5, 5)     == cell);
  TEST(mesh->cell_to_i(cell)    == 5);
  TEST(mesh->cell_to_j(cell)    == 5);
  TEST(mesh->cell_to_k(cell)    == 5);
  TEST(mesh->volume(cell)       == 1.0);

  vec_int mat_map = mesh->mesh_map("MATERIAL");
  TEST(mat_map[0]               == 0);
  TEST(mat_map[600]             == 1);

  return 0;
}


//----------------------------------------------------------------------------//
int test_Mesh1D_serialize(int argc, char *argv[])
{
#ifdef DETRAN_ENABLE_BOOST
  {
    // Get the mesh and pack it

    SP_mesh mesh = mesh_1d_fixture();
    mesh->display();
    std::ofstream ofs("mesh1d.archive");
    boost::archive::binary_oarchive oa(ofs);
    oa << mesh;
    ofs.close();
  }

  {
    // Unpack

    SP_mesh mesh;
    std::ifstream ifs("mesh1d.archive");
    boost::archive::binary_iarchive ia(ifs);
    ia >> mesh;
    ifs.close();
    mesh->display();

    // Test

    TEST(mesh->number_cells()     == 10);
    TEST(mesh->number_cells_x()   == 10);
    TEST(mesh->number_cells_y()   == 1);
    TEST(mesh->number_cells_z()   == 1);
    TEST(mesh->dx(0)              == 1.0);
    TEST(mesh->dy(0)              == 1.0);
    TEST(mesh->dz(0)              == 1.0);
    TEST(mesh->dimension()        == 1);
    int cell = 5;
    TEST(mesh->index(5)           == cell);
    TEST(mesh->cell_to_i(cell)    == 5);
    TEST(mesh->cell_to_j(cell)    == 0);
    TEST(mesh->cell_to_k(cell)    == 0);
    TEST(mesh->volume(cell)       == 1.0);
    vec_int mat_map = mesh->mesh_map("MATERIAL");
    TEST(mat_map[0]               == 0);
    TEST(mat_map[5]               == 1);
    TESTFALSE(mesh->mesh_map_exists("SHOULDNOTEXIST"));
  }
#endif
  return 0;
}

//----------------------------------------------------------------------------//
//              end of test_CartesianMesh.cc
//----------------------------------------------------------------------------//
