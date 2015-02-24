//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  test_ColorMap.cc
 *  @brief Test of ColorMap class
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

// LIST OF TEST FUNCTIONS
#define TEST_LIST            \
        FUNC(test_ColorMap)

#include "utilities/TestDriver.hh"
#include "utilities/ColorMap.hh"
#include <iostream>
#include <cstdio>
using namespace detran_test;
using namespace detran_geometry;
using namespace detran_utilities;
using namespace std;

int main(int argc, char *argv[])
{
  RUN(argc, argv);
}

//----------------------------------------------------------------------------//
// TEST DEFINITIONS
//----------------------------------------------------------------------------//

int test_ColorMap(int argc, char *argv[])
{
  vec_dbl values = linspace(0.0, 1.0, 10);
  ColorMap::vec_rgb colors = ColorMap::color(ColorMap::HOT, values);
  std::cout << colors.size() << std::endl;

  for (int i = 0; i < colors.size(); ++i)
  {
    printf(" %12.4f  %16.8f  %16.8f  %16.8f \n", values[i],
          (double)colors[i].r / 255.,
          (double)colors[i].g / 255.,
          (double)colors[i].b / 255.);
  }

  TEST((int)ColorMap::hex_to_rgb(ColorMap::darkblue).r ==   0);
  TEST((int)ColorMap::hex_to_rgb(ColorMap::darkblue).g ==   0);
  TEST((int)ColorMap::hex_to_rgb(ColorMap::darkblue).b == 139);

  TEST((int)ColorMap::hex_to_rgb(ColorMap::azure).r ==   0);
  TEST((int)ColorMap::hex_to_rgb(ColorMap::azure).g == 127);
  TEST((int)ColorMap::hex_to_rgb(ColorMap::azure).b == 255);

  return 0;
}

//---------------------------------------------------------------------------//
//              end of PPMPOutput.cc
//---------------------------------------------------------------------------//
