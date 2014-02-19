//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   DiscreteToMoment.cc
 *  @brief  DiscreteToMoment member definitions
 *  @note   Copyright (c) 2014 Jeremy Roberts
 */
//----------------------------------------------------------------------------//


#include "DiscreteToMoment.hh"
#include "SphericalHarmonics.hh"

namespace detran_angle
{

//----------------------------------------------------------------------------//
DiscreteToMoment::DiscreteToMoment(SP_momentindexer indexer)
  : d_indexer(indexer)
  , d_number_angles(0)
{
  Require(d_indexer);

  d_legendre_order = d_indexer->legendre_order();
  d_number_moments = d_indexer->number_moments();
}

//----------------------------------------------------------------------------//
void DiscreteToMoment::build(SP_quadrature q)
{
  Require(q);
  Require(q->dimension() >= 1 && q->dimension() <= 3);

  // Store the angular mesh.
  d_quadrature = q;

  // Clear the existing M matrix.
  d_D.clear();
  Assert(d_D.empty());

  // Resize D.  Note, it's stored in transpose so that moments are the inner
  // loop as will be the typical use.
  d_number_angles = d_quadrature->number_angles();
  Ensure(d_number_angles > 0);
  d_D.resize(d_number_angles, D_Col(d_number_moments));

  for (size_t i = 0; i < d_number_moments; ++i)
  {
    if (d_quadrature->dimension() == 1)
      calc_col_1d(i);
    else if (d_quadrature->dimension() == 2)
      calc_col_2d(i);
    else
      calc_col_3d(i);
  }
}

//----------------------------------------------------------------------------//
void DiscreteToMoment::calc_col_3d(const size_t i)
{
  size_t l = d_indexer->l(i);
  int m = d_indexer->m(i);
  for (size_t o = 0; o < d_quadrature->number_octants(); ++o)
  {
    for (size_t a = 0; a < d_quadrature->number_angles_octant(); ++a)
    {
      size_t angle = d_quadrature->index(o, a);
      D_Col &col = d_D[angle];
      double mu  = d_quadrature->mu(o, a);
      double eta = d_quadrature->eta(o, a);
      double xi  = d_quadrature->xi(o, a);
      double w   = d_quadrature->weight(a);
      col[i] = SphericalHarmonics::Y_lm(l, m, mu, eta, xi) * w;
    }
  }
}

//----------------------------------------------------------------------------//
void DiscreteToMoment::calc_col_2d(const size_t i)
{
  size_t l = d_indexer->l(i);
  int m = d_indexer->m(i);
  for (size_t o = 0; o < d_quadrature->number_octants(); ++o)
  {
    for (size_t a = 0; a < d_quadrature->number_angles_octant(); ++a)
    {
      size_t angle = d_quadrature->index(o, a);
      D_Col &col = d_D[angle];
      double mu  = d_quadrature->mu(o, a);
      double eta = d_quadrature->eta(o, a);
      double xi  = std::sqrt(1.0 - mu * mu - eta * eta);
      double w   = d_quadrature->weight(a);
      col[i] = SphericalHarmonics::Y_lm(l, m, mu, eta, xi) * w;
    }
  }
}

//----------------------------------------------------------------------------//
void DiscreteToMoment::calc_col_1d(const size_t i)
{
  size_t l = d_indexer->l(i);
  int m = d_indexer->m(i);
  for (size_t o = 0; o < d_quadrature->number_octants(); ++o)
  {
    for (size_t a = 0; a < d_quadrature->number_angles_octant(); ++a)
    {
      size_t angle = d_quadrature->index(o, a);
      D_Col &col = d_D[angle];
      double mu  = d_quadrature->mu(o, a);
      double w   = d_quadrature->weight(a);
      col[i] = SphericalHarmonics::Y_lm(l, mu) * w;
    }
  }
}

//----------------------------------------------------------------------------//
// Operator(cardinal_angle_index, cardinal_moment_index)
const double& DiscreteToMoment::
operator()(const size_t moment, const size_t angle) const
{
  Require(angle < d_number_angles);
  Require(moment < d_number_moments);
  return d_D[angle][moment];
}

//----------------------------------------------------------------------------//
// Operator(cardinal_angle_index, legendre_degree, legendre_order)
const double& DiscreteToMoment::
operator()(const size_t l,
           const int    m,
           const size_t angle) const
{
  // Moment cardinal index
  size_t moment = d_indexer->index(l, m);
  return (*this)(moment, angle);
}

//----------------------------------------------------------------------------//
// Operator(octant_index, angle_in_octant, legendre_degree, legendre_order)
const double& DiscreteToMoment::
operator()(const size_t l,
           const int    m,
           const size_t o,
           const size_t a) const
{
  // Angle cardinal index
  size_t angle = d_quadrature->index(o, a);
  // Moment cardinal index
  size_t moment = d_indexer->index(l, m);
  return (*this)(moment, angle);
}

} // end namespace detran_angle

//----------------------------------------------------------------------------//
//              end of DiscreteToMoment.cc
//----------------------------------------------------------------------------//
