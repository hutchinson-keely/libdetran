#------------------------------------------------------------------------------#
# src/geometry/CMakeLists.txt
#------------------------------------------------------------------------------#

SET(LIB_NAME "geometry")

#-----------------------------------------------------------------------------#
# SOURCE
#-----------------------------------------------------------------------------#

SET(SRC
    Mesh.cc
    CartesianMesh.cc
    PinCell.cc
    Assembly.cc
    Core.cc
    Segment.cc
    Track.cc
    TrackDB.cc
    Tracker.cc
    QuadraticSurface.cc
    Point.cc
    CSG.cc
    Region.cc
    RegionFactory.cc
    Geometry.cc
    GeometryPlotter.cc
)

#-----------------------------------------------------------------------------#
# LINKED LIBRARIES
#-----------------------------------------------------------------------------#

set(LINKED_LIBS
    angle
    callow
    utilities
)

#-----------------------------------------------------------------------------#
# LIBRARY
#-----------------------------------------------------------------------------#

setup_detran_library(${LIB_NAME} "${SRC}" "${LINKED_LIBS}")
        