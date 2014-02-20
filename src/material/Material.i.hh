//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   Material.i.hh
 *  @author Jeremy Roberts
 *  @brief  Material class inline member definitions.
 */
//----------------------------------------------------------------------------//

#ifndef detran_material_MATERIAL_I_HH_
#define detran_material_MATERIAL_I_HH_

#include "utilities/DBC.hh"

namespace detran_material
{

//----------------------------------------------------------------------------//
inline double Material::sigma_t(const size_t m, const size_t g) const
{
  Require(m < d_number_materials);
  Require(g < d_number_groups);
  return d_sigma_t[g][m];
}

//----------------------------------------------------------------------------//
inline double Material::sigma_a(const size_t m, const size_t g) const
{
  Require(m < d_number_materials);
  Require(g < d_number_groups);
  return d_sigma_a[g][m];
}

//----------------------------------------------------------------------------//
inline double Material::nu_sigma_f(const size_t m, const size_t g) const
{
  Require(m < d_number_materials);
  Require(g < d_number_groups);
  return d_nu_sigma_f[g][m];
}

//----------------------------------------------------------------------------//
inline double Material::sigma_f(const size_t m, const size_t g) const
{
  Require(m < d_number_materials);
  Require(g < d_number_groups);
  return d_sigma_f[g][m];
}

//----------------------------------------------------------------------------//
inline double Material::nu(const size_t m, const size_t g) const
{
  Require(m < d_number_materials);
  Require(g < d_number_groups);
  return d_nu[g][m];
}

//----------------------------------------------------------------------------//
inline double Material::chi(const size_t m, const size_t g) const
{
  Require(m < d_number_materials);
  Require(g < d_number_groups);
  return d_chi[g][m];
}

//----------------------------------------------------------------------------//
inline double Material::sigma_s(const size_t m, const size_t g,
                                const size_t gp, const size_t l) const
{
  Require(m < d_number_materials);
  Require(g < d_number_groups);
  Require(gp < d_number_groups);
  Require(l <= d_legendre_order);
  return d_sigma_s[g][gp][m][l];
}

//----------------------------------------------------------------------------//
inline double Material::diff_coef(const size_t m, const size_t g) const
{
  Require(m < d_number_materials);
  Require(g < d_number_groups);
  return d_diff_coef[g][m];
}

} // end namespace detran

#endif /* detran_material_MATERIAL_I_HH_ */

//----------------------------------------------------------------------------//
//              end of file Material.i.hh
//----------------------------------------------------------------------------//
