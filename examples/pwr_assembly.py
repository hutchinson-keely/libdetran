from detran.geometry import *
import detran
import time



def make_geometry() :

    # Three cell types: moderator, fuel I, and fuel II
    P = Point(1.26)
    mat_M = detran.utilities.vec_int(1, 0)
    mat_M[0] = 0
    mat_1 = detran.utilities.vec_int(3, 0)
    mat_1[0] = 1
    mat_1[1] = 2
    mat_2 = detran.utilities.vec_int(3, 0)
    mat_2[0] = 3
    mat_2[1] = 4
    radii = detran.utilities.vec_dbl(2, 0)
    radii[0] = 0.50;
    radii[1] = 0.54;
    
    M   = PinCell(P, mat_M)
    F_1 = PinCell(P, mat_1, radii, PinCell.DIVISION_HV_DIAG)
    F_2 = PinCell(P, mat_2, radii, PinCell.DIVISION_HV_DIAG)

    n = 17
    a = Assembly(n, n)
    
    pin_map = detran.utilities.vec_int( 
        [ 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1,        
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1
        ]
    )
    a.add_pincell(M)
    a.add_pincell(F_1)
    a.add_pincell(F_2)
    a.set_pincell_map(pin_map)

    regions = RegionFactory.CreateAssembly(a)
    print " # reg = ", regions.size() 
     
    geo = Geometry(n*1.26, n*1.26, 0.0);
    for i in range(0, regions.size()) :
      geo.add_region(regions[i])
    geo.finalize()
    return geo;

geo = make_geometry()

plotter = GeometryPlotter("pwr")
plotter.initialize(geo, 0.01)
plotter.draw_geometry(geo, True, 5)

