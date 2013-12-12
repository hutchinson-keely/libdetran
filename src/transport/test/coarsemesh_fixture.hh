//----------------------------------*-C++-*----------------------------------//
/**
 *  @file  coarsemesh_fixture.hh
 *  @brief coarsemesh_fixture
 *  @note  Copyright (C) 2012-2013 Jeremy Roberts
 */
//---------------------------------------------------------------------------//

#ifndef COARSEMESH_FIXTURE_HH_
#define COARSEMESH_FIXTURE_HH_

#include "CoarseMesh.hh"
#include "geometry/CartesianMesh.hh"

namespace detran_test
{

detran::CoarseMesh::SP_coarsemesh coarsemesh_1d()
{
  using namespace detran_geometry;
  using detran::CoarseMesh;
  using detran_utilities::vec_dbl;
  using detran_utilities::vec_int;

  // Coarse mesh edges
  vec_dbl cm(3, 0.0);
  cm[1] = 5.0;
  cm[2] = 10.0;

  // Fine mesh counts
  vec_int fm(2, 5);
  fm[1] = 10;

  // Material (homogeneous)
  vec_int mt(2, 0);

  // Create the fine mesh
  CartesianMesh::SP_mesh mesh = Mesh1D::Create(fm, cm, mt);
  mesh->display();

  // Create the coarse mesh and return
  CoarseMesh::SP_coarsemesh coarse(new CoarseMesh(mesh, 2));
  return coarse;

}

detran::CoarseMesh::SP_coarsemesh coarsemesh_2d()
{
  using namespace detran_geometry;
  using detran::CoarseMesh;
  using detran_utilities::vec_dbl;
  using detran_utilities::vec_int;


  // Coarse mesh edges
  vec_dbl cm(3, 0.0);
  cm[1] = 5.0;
  cm[2] = 10.0;

  // Fine mesh counts
  vec_int fm(2, 5);
  fm[1] = 10;

  // Material (homogeneous)
  vec_int mt(4, 0);

  // Create the fine mesh
  CartesianMesh::SP_mesh mesh = Mesh2D::Create(fm, fm, cm, cm, mt);

  // Create the coarse mesh and return
  CoarseMesh::SP_coarsemesh coarse(new CoarseMesh(mesh, 2));
  return coarse;

}


detran::CoarseMesh::SP_coarsemesh coarsemesh_2d_b()
{
  using namespace detran_geometry;
  using detran::CoarseMesh;
  using detran_utilities::vec_dbl;
  using detran_utilities::vec_int;

  // Coarse mesh edges
  vec_dbl cm(3, 0.0);
  cm[1] = 5.0;
  cm[2] = 5.0;

  // Fine mesh counts
  vec_int fm(2, 2);

  // Material (homogeneous)
  vec_int mt(4, 0);

  // Create the fine mesh
  CartesianMesh::SP_mesh mesh = Mesh2D::Create(fm, fm, cm, cm, mt);

  // Create the coarse mesh and return
  CoarseMesh::SP_coarsemesh coarse(new CoarseMesh(mesh, 2));
  return coarse;

}

detran::CoarseMesh::SP_coarsemesh coarsemesh_3d()
{
  using namespace detran_geometry;
  using detran::CoarseMesh;
  using detran_utilities::vec_dbl;
  using detran_utilities::vec_int;

  // Coarse mesh edges
  vec_dbl cm(3, 0.0);
  cm[1] = 5.0;
  cm[2] = 10.0;

  // Fine mesh counts
  vec_int fm(2, 5);
  fm[1] = 10;

  // Material (homogeneous)
  vec_int mt(8, 0);

  // Create the fine mesh
  CartesianMesh::SP_mesh mesh = Mesh3D::Create(fm, fm, fm, cm, cm, cm, mt);

  // Create the coarse mesh and return
  CoarseMesh::SP_coarsemesh coarse(new CoarseMesh(mesh, 2));
  return coarse;

}

detran::CoarseMesh::SP_coarsemesh coarsemesh_3d_b()
{
  using namespace detran_geometry;
  using detran::CoarseMesh;
  using detran_utilities::vec_dbl;
  using detran_utilities::vec_int;


  // Coarse mesh edges
  vec_dbl cm(3, 0.0);
  cm[1] = 5.0;
  cm[2] = 10.0;

  // Fine mesh counts
  vec_int fm(2, 2);

  // Material (homogeneous)
  vec_int mt(8, 0);

  // Create the fine mesh
  CartesianMesh::SP_mesh mesh = Mesh3D::Create(fm, fm, fm, cm, cm, cm, mt);

  // Create the coarse mesh and return
  CoarseMesh::SP_coarsemesh coarse(new CoarseMesh(mesh, 2));
  return coarse;

}

} // end namespace detran

#endif // COARSEMESH_FIXTURE_HH_ 

//---------------------------------------------------------------------------//
//              end of file coarsemesh_fixture.hh
//---------------------------------------------------------------------------//
