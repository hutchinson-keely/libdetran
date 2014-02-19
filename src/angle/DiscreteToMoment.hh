//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   DiscreteToMoment.hh
 *  @brief  DiscreteToMoment class definition.
 *  @note   Copyright (c) 2014 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef detran_angle_DISCRETE_TO_MOMENT_HH_
#define detran_angle_DISCRETE_TO_MOMENT_HH_

#include "Quadrature.hh"
#include "MomentIndexer.hh"
#include "utilities/Definitions.hh"
#include "utilities/SP.hh"
#include <vector>

namespace detran_angle
{

//----------------------------------------------------------------------------//
/**
 *  @class DiscreteToMoment
 *  @brief Converts discrete-valued unknowns to moment values
 *
 *  For a discrete angular unknown, d, the corresponding
 *  moment representation, m, is m = D*d.
 *
 */
//----------------------------------------------------------------------------//
class ANGLE_EXPORT DiscreteToMoment
{

public:

  //--------------------------------------------------------------------------//
  // TYPEDEFS
  //--------------------------------------------------------------------------//

  typedef detran_utilities::SP<DiscreteToMoment>  SP_DtoM;
  typedef MomentIndexer::SP_momentindexer         SP_momentindexer;
  typedef Quadrature::SP_quadrature               SP_quadrature;
  typedef detran_utilities::size_t                size_t;
  typedef detran_utilities::vec_dbl               D_Col;
  typedef std::vector<D_Col>                      Operator_D;

  //--------------------------------------------------------------------------//
  // CONSTRUCTOR & DESTRUCTOR
  //--------------------------------------------------------------------------//

  /**
   *  @brief Constructor.
   *  @param indexer   Indexer for spherical harmonic orders
   */
  explicit DiscreteToMoment(SP_momentindexer indexer);

  /// SP contructor
  static SP_DtoM Create(SP_momentindexer indexer, SP_quadrature q)
  {
    SP_DtoM d(new DiscreteToMoment(indexer));
    d->build(q);
    return d;
  }

  //--------------------------------------------------------------------------//
  // PUBLIC INTERFACE
  //--------------------------------------------------------------------------//

  /**
   *  @brief Build the discrete-to-moment operator.
   *
   *  Keeping the actual construction outside the constructor allows
   *  us to rebuild the operator for different angular solves using
   *  the same spatial grid. This is useful for coupled forward and
   *  adjoint solves, compact testing of quadrature sets, and potential
   *  angular multigrid schemes.
   *
   *  @param     q     Pointer to quadrature
   */
  void build(SP_quadrature q);

  /**
   *  @brief Return an element from \f$ D\f$.
   *
   *  @param     moment      Cardinal moment index, i.e. row
   *  @param     angle       Cardinal angle index, i.e. column
   *  @return                Element of operator.
   */
  const double& operator()(const size_t moment, const size_t angle) const;

  /**
   *  @brief Return an element from \f$ D\f$.
   *
   *  @param     l           Legendre degree
   *  @param     m           Legendre order
   *  @param     angle       Cardinal angle index, i.e. column
   *  @return                Element of operator.
   */
  const double& operator()(const size_t l,
                           const int    m,
                           const size_t angle) const;

  /**
   *  @brief Return an element from \f$ D\f$.
   *
   *  @param     l           Legendre degree
   *  @param     m           Legendre order
   *  @param     o           Octant index
   *  @param     a           Angle index (within octant)
   *  @return                Element of operator.
   */
  const double& operator()(const size_t l,
                           const int    m,
                           const size_t o,
                           const size_t a) const;

  /**
   *  @brief Return a row of the operator.
   *
   *  @param     angle       Cardinal angle index, i.e. column index.
   *  @return                A column
   */
  const D_Col& get_col(const size_t angle) const
  {
    Require(angle < d_number_moments);
    return d_D[angle];
  }

  /// Return number of angles (length of row in \f$\mathbf{M}\f$).
  size_t row_size() const
  {
    return d_number_angles;
  }

  /// Return number of moments (length of column in \f$\mathbf{M}\f$).
  size_t column_size() const
  {
    return d_number_moments;
  }

private:

  //--------------------------------------------------------------------------//
  // DATA
  //--------------------------------------------------------------------------//

  /// Moment indexer
  SP_momentindexer d_indexer;
  /// Legendre order of angular flux expansion.  Must be >= scattering order.
  size_t d_legendre_order;
  /// Number of angular moments.
  size_t d_number_moments;
  /// Angular mesh.
  SP_quadrature d_quadrature;
  /// Number of angles (not const so that we can change angular meshes)
  size_t d_number_angles;
  /// Moments-to-discrete operator \f$\mathbf{D}\f$.
  Operator_D d_D;

  //--------------------------------------------------------------------------//
  // IMPLEMENTATION
  //--------------------------------------------------------------------------//

  /**
   *  @brief Calculate one col of \f$\mathbf{D}\f$.
   *
   *  @param     moment      Cardinal moment index
   */
  void calc_col_1d(const size_t i);
  void calc_col_2d(const size_t i);
  void calc_col_3d(const size_t i);
};

} // end namespace detran_angle

#endif /* detran_angle_DISCRETE_TO_MOMENT_HH_ */

//----------------------------------------------------------------------------//
//              end of MomentToDiscrete.hh
//----------------------------------------------------------------------------//
