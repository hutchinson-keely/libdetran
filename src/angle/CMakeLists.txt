#------------------------------------------------------------------------------#
# src/angle/CMakeLists.txt
#------------------------------------------------------------------------------#

set(LIB_NAME "angle")

#------------------------------------------------------------------------------#
# SOURCE
#------------------------------------------------------------------------------#

set(SRC
    AbuShumaysDoubleRange.cc
    AbuShumaysQuadrupleRange.cc
    LevelSymmetric.cc
    MomentToDiscrete.cc
    DiscreteToMoment.cc
    Quadrature.cc
    QuadratureFactory.cc
    SphericalHarmonics.cc
    TabuchiYamamoto.cc
    UniformEqual.cc
    MomentIndexer.cc
    PolarQuadrature.t.cc
    AzimuthalQuadrature.t.cc
    ProductQuadrature.cc
    ProductQuadratureAP.t.cc
)

#------------------------------------------------------------------------------#
# LINKED LIBRARIES
#------------------------------------------------------------------------------#

set(LINKED_LIBS
    utilities
)

#------------------------------------------------------------------------------#
# LIBRARY
#------------------------------------------------------------------------------#

setup_detran_library(${LIB_NAME} "${SRC}" "${LINKED_LIBS}")
