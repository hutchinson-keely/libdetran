//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  test_GeometryPlotter.cc
 *  @brief Test of GeometryPlotter class
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

// LIST OF TEST FUNCTIONS
#define TEST_LIST                                \
        FUNC(test_GeometryPlotter_mesh)          \
        FUNC(test_GeometryPlotter_geo_pincell)   \
        FUNC(test_GeometryPlotter_geo_assembly)


#include "utilities/TestDriver.hh"
#include "geometry/GeometryPlotter.hh"
#include "geometry/test/csg_fixture.hh"
#include "callow/utils/Initialization.hh"
#include "utilities/ColorMap.hh"
#include <iostream>

using namespace detran_test;
using namespace detran_geometry;
using namespace detran_utilities;
using namespace std;

int main(int argc, char *argv[])
{
  callow::Callow::initialize(argc, argv);
  RUN(argc, argv);
}

//----------------------------------------------------------------------------//
// TEST DEFINITIONS
//----------------------------------------------------------------------------//

int test_GeometryPlotter_mesh(int argc, char *argv[])
{

  return 0;
}

int test_GeometryPlotter_geo_pincell(int argc, char *argv[])
{
  // simple pin cell
  {
    GeometryPlotter::SP_geometry geo = test_2D_pincell_simple();
    GeometryPlotter ppm("test_2D_pincell_simple");
    ppm.initialize(geo, 0.01);
    ppm.draw_geometry(geo, true);
  }

  // complex pin cell
  {
    GeometryPlotter::SP_geometry geo = test_2D_pincell_complex();
    GeometryPlotter ppm("test_2D_pincell_complex");
    ppm.initialize(geo, 0.001);
    ppm.draw_geometry(geo, true);
  }

  // complex pin cell
  {
    GeometryPlotter::SP_geometry geo = test_2D_pincell_via_factory(3, 3);
    //GeometryPlotter::SP_geometry geo = test_2D_assembly(8);
    GeometryPlotter ppm("test_2D_pincell_via_factory");
    ppm.initialize(geo, 0.01);
    ppm.draw_geometry(geo, true, ColorMap::RANDOM);
  }

  return 0;
}

int test_GeometryPlotter_geo_assembly(int argc, char *argv[])
{
  GeometryPlotter::SP_geometry geo = test_2D_assembly(8);
  GeometryPlotter ppm("test_2D_assembly");
  ppm.initialize(geo, 0.01);
  ppm.draw_geometry(geo, true, ColorMap::RANDOM);
  return 0;
}

//---------------------------------------------------------------------------//
//              end of PPMPOutput.cc
//---------------------------------------------------------------------------//
