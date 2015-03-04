from detran.geometry import *
import detran
import time



def make_geometry() :

    # Three cell types: moderator, fuel I, and fuel II
    P = Point(1.26)
    mat = detran.utilities.vec_int(3, 0)
    mat[1] = 1
    mat[2] = 2
    radii = detran.utilities.vec_dbl(2, 0)
    radii[0] = 0.50;
    radii[1] = 0.54;
    pin = PinCell(P, mat, radii, PinCell.DIVISION_HV_DIAG)
    regions = RegionFactory.CreatePinCell(pin)
    print " # reg = ", regions.size() 
     
    geo = Geometry(1.26, 1.26, 0.0);
    for i in range(0, regions.size()) :
      geo.add_region(regions[i])
    geo.finalize()
    return geo;

geo = make_geometry()

plotter = GeometryPlotter("pincell")
plotter.initialize(geo, 0.001)
plotter.draw_geometry(geo, True, 5)

