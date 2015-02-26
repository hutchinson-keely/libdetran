//----------------------------------*-C++-*----------------------------------//
/**
 *  @file   BoundaryConditionMOC.hh
 *  @brief  BoundaryConditionMOC
 *  @author Jeremy Roberts
 *  @date   Jun 26, 2012
 */
//---------------------------------------------------------------------------//

#ifndef detran_BOUNDARYCONDITIONMOC_HH_
#define detran_BOUNDARYCONDITIONMOC_HH_

#include "BoundaryMOC.hh"

namespace detran
{

// Forward declare boundary and traits.
template <class D> class BoundaryMOC;

//---------------------------------------------------------------------------//
/**
 *  @class BoundaryCondition
 *  @brief Boundary condition for a surface.
 */
//---------------------------------------------------------------------------//

/// \todo template on the Boundary type!!

template <class D>
class BoundaryConditionMOC
{

public:

  //-------------------------------------------------------------------------//
  // TYPEDEFS
  //-------------------------------------------------------------------------//

  typedef detran_utilities::SP<BoundaryConditionMOC>    SP_bc;
  typedef BoundaryMOC<D>                                Boundary_T;
  typedef typename Boundary_T::SP_boundary              SP_boundary;
  typedef typename Boundary_T::SP_input                 SP_input;
  typedef typename Boundary_T::SP_geometry              SP_geometry;
  typedef typename Boundary_T::SP_trackdb               SP_trackdb;
  typedef detran_utilities::vec_dbl                     boundary_flux_type;
  typedef typename Boundary_T::size_t                   size_t;

  //-------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //-------------------------------------------------------------------------//

  BoundaryConditionMOC(BoundaryMOC<D>& boundary,
                       const size_t    side,
                       SP_input        input,
                       SP_geometry     geo,
                       SP_trackdb      tracks)
    : d_boundary(boundary)
    , d_side(side)
    , d_input(input)
    , d_geometry(geo)
    , d_tracks(tracks)
  {
    Require(d_side <= D::dimension*2);
    Require(d_mesh);
    Require(d_quadrature);
  }

  /// Virtual destructor
  virtual ~BoundaryConditionMOC(){}

  //-------------------------------------------------------------------------//
  // ABSTRACT INTERFACE -- ALL MOC BOUNDARY CONDITIONS MUST IMPLEMENT THESE
  //-------------------------------------------------------------------------//

  /// Set initial and/or fixed boundary condition.
  virtual void set(const size_t g) = 0;

  /// Update a boundary following a sweep.
  virtual void update(const size_t g) = 0;

  /// Update a boundary for a given angle following a sweep.
  virtual void update(const size_t g, const size_t o, const size_t a) = 0;

protected:

  //-------------------------------------------------------------------------//
  // DATA
  //-------------------------------------------------------------------------//

  /// Boundary flux container. \todo Is there a way around using a reference?
  Boundary_T& d_boundary;
  /// My surface.
  const size_t d_side;
  /// Input
  SP_input d_input;
  /// Geometry
  SP_geometry d_geometry;
  /// Track database.
  SP_trackdb d_tracks;

};

} // end namespace detran

#endif // detran_BOUNDARYCONDITIONMOC_HH_

//---------------------------------------------------------------------------//
//              end of file BoundaryConditionMOC.hh
//---------------------------------------------------------------------------//
