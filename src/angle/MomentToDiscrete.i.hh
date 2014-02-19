//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   MomentToDiscrete.i.hh
 *  @brief  MomentToDiscrete inline member definitions.
 *  @note   Copyright (c) 2014 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef detran_angle_MOMENT_TO_DISCRETE_I_HH_
#define detran_angle_MOMENT_TO_DISCRETE_I_HH_

#include "SphericalHarmonics.hh"
#include "utilities/Constants.hh"
#include "utilities/DBC.hh"
#include "utilities/SoftEquivalence.hh"
#include <cmath>
#include <iostream>

namespace detran_angle
{

//----------------------------------------------------------------------------//
// Operator(cardinal_angle_index, cardinal_moment_index)
inline const double& MomentToDiscrete::
operator()(const size_t angle, const size_t moment) const
{
  Require(angle < d_number_angles);
  Require(moment < d_number_moments);
  return d_M[angle][moment];
}

//----------------------------------------------------------------------------//
// Operator(cardinal_angle_index, legendre_degree, legendre_order)
inline const double& MomentToDiscrete::
operator()(const size_t angle, const size_t l, const size_t m) const
{
  // Moment cardinal index
  size_t moment = d_indexer->index(l, m);
  return (*this)(angle, moment);
}

//----------------------------------------------------------------------------//
// Operator(octant_index, angle_in_octant, legendre_degree, legendre_order)
inline const double& MomentToDiscrete::
operator()(const size_t o, const size_t a,
           const size_t l, const size_t m) const
{
  // Angle cardinal index
  size_t angle = d_quadrature->index(o, a);
  // Moment cardinal index
  size_t moment = d_indexer->index(l, m);
  return (*this)(angle, moment);
}

} // end namespace detran_angle

#endif /* detran_angle_MOMENT_TO_DISCRETE_I_HH_ */

//----------------------------------------------------------------------------//
//              end of MomentToDiscrete.i.hh
//----------------------------------------------------------------------------//
