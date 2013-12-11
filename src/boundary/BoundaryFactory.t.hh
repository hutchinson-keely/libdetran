//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   BoundaryFactory.t.hh
 *  @author robertsj
 *  @date   Jan 30, 2013
 *  @brief  BoundaryFactory.t class definition.
 */
//----------------------------------------------------------------------------//

#ifndef detran_BOUNDARYFACTORY_T_HH_
#define detran_BOUNDARYFACTORY_T_HH_

#include "BoundaryFactory.hh"
#include "BoundaryDiffusion.hh"
#include "BoundarySN.hh"
#include "BoundaryMOC.hh"
//
#include "Reflective.hh"
#include "Vacuum.hh"
#include "FixedBoundary.hh"
#include "Periodic.hh"

namespace detran
{

//----------------------------------------------------------------------------//
// DIFFUSION
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
template <typename D>
class BoundaryFactory<D, BoundaryDiffusion>
{
public:
  typedef BoundaryDiffusion<D>                Boundary_T;
  typedef typename Boundary_T::SP_boundary    SP_boundary;
  typedef typename Boundary_T::SP_input       SP_input;
  typedef typename Boundary_T::SP_mesh        SP_mesh;
  typedef typename Boundary_T::SP_quadrature  SP_quadrature;
  typedef typename Boundary_T::size_t         size_t;

  static SP_boundary build(SP_input       input,
                           SP_mesh        mesh,
                           SP_quadrature  quad,
                           const size_t   ng);
};

//----------------------------------------------------------------------------//
template <class D>
typename BoundaryFactory<D, BoundaryDiffusion>::SP_boundary
BoundaryFactory<D, BoundaryDiffusion>::build(SP_input       input,
                                             SP_mesh        mesh,
                                             SP_quadrature  quad,
                                             const size_t   ng)
{
  SP_boundary b(new BoundaryDiffusion<D>(input, mesh, ng));
  return b;
}

//----------------------------------------------------------------------------//
// SN
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
template <typename D>
class BoundaryFactory<D, BoundarySN>
{

public:

  typedef BoundarySN<D>                       Boundary_T;
  typedef typename Boundary_T::SP_boundary    SP_boundary;
  typedef typename Boundary_T::SP_input       SP_input;
  typedef typename Boundary_T::SP_mesh        SP_mesh;
  typedef typename Boundary_T::SP_quadrature  SP_quadrature;
  typedef typename Boundary_T::size_t         size_t;

  static SP_boundary build(SP_input       input,
                           SP_mesh        mesh,
                           SP_quadrature  quad,
                           const size_t   ng);
};

//----------------------------------------------------------------------------//
template <class D>
typename BoundaryFactory<D, BoundarySN>::SP_boundary
BoundaryFactory<D, BoundarySN>::build(SP_input      input,
                                      SP_mesh       mesh,
                                      SP_quadrature quad,
                                      const size_t  ng)
{
  SP_boundary b(new BoundarySN<D>(input, mesh, quad, ng));

  using namespace detran_geometry;

  // boundary conditions
  std::vector<std::string> names(6);
  names[CartesianMesh::WEST]   = "bc_west";
  names[CartesianMesh::EAST]   = "bc_east";
  names[CartesianMesh::SOUTH]  = "bc_south";
  names[CartesianMesh::NORTH]  = "bc_north";
  names[CartesianMesh::BOTTOM] = "bc_bottom";
  names[CartesianMesh::TOP]    = "bc_top";

  // Assign boundary conditions.
  for(int side = 0; side < 2*D::dimension; side++)
  {
    // Vacuum is default.
    std::string type = "vacuum";
    if (input->check(names[side]))
      type = input->template get<std::string>(names[side]);
    typename Boundary_T::SP_bc bc;
    if (type == "vacuum")
      bc = new Vacuum<D>(b, side, input, mesh, quad);
    else if (type == "reflect")
      bc = new Reflective<D>(b, side, input, mesh, quad);
    else if (type == "fixed")
      bc = new FixedBoundary<D>(b, side, input, mesh, quad);
    else if (type == "periodic")
      bc = new Periodic<D>(b, side, input, mesh, quad);
    if (!bc)
    {
      type.append(" is not a supported bc type.");
      THROW(type);
      break;
    }
    b->set_bc(side, bc);
    Ensure(b->bc(side));
  }

  return b;
}

//----------------------------------------------------------------------------//
// MOC
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
template <typename D>
class BoundaryFactory<D, BoundaryMOC>
{
public:

  typedef BoundaryMOC<D>                      Boundary_T;
  typedef typename Boundary_T::SP_boundary    SP_boundary;
  typedef typename Boundary_T::SP_input       SP_input;
  typedef typename Boundary_T::SP_mesh        SP_mesh;
  typedef typename Boundary_T::SP_quadrature  SP_quadrature;
  typedef typename Boundary_T::size_t         size_t;

  static SP_boundary build(SP_input       input,
                           SP_mesh        mesh,
                           SP_quadrature  quad,
                           const size_t   ng);
};

//----------------------------------------------------------------------------//
template <class D>
typename BoundaryFactory<D, BoundaryMOC>::SP_boundary
BoundaryFactory<D, BoundaryMOC>::build(SP_input       input,
                                       SP_mesh        mesh,
                                       SP_quadrature  quad,
                                       const size_t   ng)
{
  SP_boundary b(new BoundaryMOC<D>(input, mesh, quad, ng));
  return b;
}

} // end namespace detran

#endif /* detran_BOUNDARYFACTORY_T_HH_ */

//----------------------------------------------------------------------------//
//              end of file BoundaryFactory.t.hh
//----------------------------------------------------------------------------//
