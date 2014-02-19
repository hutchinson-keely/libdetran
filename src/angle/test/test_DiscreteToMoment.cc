//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   test_DiscreteToMoment.cc
 *  @brief  Test of DiscreteToMoment class
 *  @note   Copyright (C) 2014 Jeremy Roberts.
 */
//----------------------------------------------------------------------------//

// LIST OF TEST FUNCTIONS
#define TEST_LIST                   \
        FUNC(test_DiscreteToMoment)

// Detran headers
#include "TestDriver.hh"
#include "DiscreteToMoment.hh"
#include "QuadratureFactory.hh"

using namespace detran_angle;
using namespace detran_utilities;
using namespace detran_test;
using namespace std;

int main(int argc, char *argv[])
{
  RUN(argc, argv);
}

//-----------------------------------------------//
// TEST DEFINITIONS
//-----------------------------------------------//

int test_DiscreteToMoment(int argc, char *argv[])
{
  QuadratureFactory qf;
  Quadrature::SP_quadrature q;
  InputDB::SP_input db = InputDB::Create();

  // 1d test
  {
    // Indexer
    MomentIndexer::SP_momentindexer indexer = MomentIndexer::Create(1, 1);
    // DtoM
    DiscreteToMoment DtoM(indexer);
    // Quadrature
    q = qf.build(db, 1);
    TEST(q);
    // Build
    DtoM.build(q);
    // TEST (l,m,angle)
    TEST(soft_equiv(DtoM(0, 0, 0), 1.0));
    TEST(soft_equiv(DtoM(1, 0, 0), 0.577350269189626));
  }

  // 2d test
  {
    // Indexer
    MomentIndexer::SP_momentindexer indexer = MomentIndexer::Create(2, 1);
    // MtoD
    DiscreteToMoment DtoM(indexer);
    // Quadrature
    q = qf.build(db, 2);
    TEST(q);
    // Build
    DtoM.build(q);
    // TEST
    TEST(soft_equiv(DtoM(0, 0, 0), pi));
    TEST(soft_equiv(DtoM(1,-1, 0), 0.6123724356958 * pi));
    TEST(soft_equiv(DtoM(1, 1, 0), 0.6123724356958 * pi));
  }

  // 3d test
  {
    // Indexer
    MomentIndexer::SP_momentindexer indexer = MomentIndexer::Create(3, 1);
    // MtoD
    DiscreteToMoment DtoM(indexer);
    // Quadrature
    q = qf.build(db, 3);
    TEST(q);
    // Build
    DtoM.build(q);
    // TEST
    TEST(soft_equiv(DtoM(0, 0, 0), 0.5*pi));
    TEST(soft_equiv(DtoM(1,-1, 0), 0.6123724356958 * 0.5*pi));
    TEST(soft_equiv(DtoM(1, 0, 0), 0.5             * 0.5*pi));
    TEST(soft_equiv(DtoM(1, 1, 0), 0.6123724356958 * 0.5*pi));
  }

  return 0;
}


//----------------------------------------------------------------------------//
//              end of test_MomentToDiscrete.cc
//----------------------------------------------------------------------------//
