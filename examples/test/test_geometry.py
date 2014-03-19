import unittest
import sys
import numpy as np
from detran.geometry import *
import detran
import time

def soft_equiv(a, b, tol = 1.0e-12) :
    if np.abs(a-b) < tol :
        return True
    return False

class TestGeometry(unittest.TestCase) :

    def setUp(self) :
        #Callow.initialize(sys.argv)
        pass
    
    def cartesianmesh(self) :
        fm = [3,3]
        cm = [0.0,1.0,2.0]
        # 1D
        mesh = Mesh1D.Create(fm, cm,[0,1])
        mesh.display()
        # 2D
        mesh = Mesh2D.Create(fm, fm, cm, cm, [0,1,0,1])
        mesh.display()
        # 3D
        mesh = Mesh3D.Create(fm, fm, fm, cm, cm, cm, [0,0,0,0,1,1,1,1])
        mesh.display()
        
    def test_geometry(self) :
        geo = Geometry(1.0, 1.0)
        # cylinder
        QSF = QuadraticSurfaceFactory
        c  = QSF.CreateCylinderZ(0.5,0.5,0.4)
        x0 = QSF.CreatePlaneX(0.0)
        x1 = QSF.CreatePlaneX(1.0)
        y0 = QSF.CreatePlaneY(0.0)
        y1 = QSF.CreatePlaneY(1.0)
        r = Region(0, Point(0,0), Point(1,1))
        r.append(c, False)
        geo.add_region(r)
        r = Region(1, Point(0,0), Point(1,1))
        r.append(c,  True) 
        r.append(x0, True)
        r.append(x1, False)
        r.append(y0, True)
        r.append(y1, False)
        geo.add_region(r)
        geo.finalize()
        geo.display()
        
        print geo.find_cell(Point(0.5,0.5))
        print geo.find_cell(Point(0.9,0.9))
        print geo.find_cell(Point(1.1,1.1))
        
if __name__ == '__main__':
    unittest.main()
