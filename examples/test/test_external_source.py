import unittest
import sys
import numpy as np
from detran.utilities import *
from detran.external_source import *
from detran.geometry import *
from detran.angle import *
import detran
import time

def soft_equiv(a, b, tol = 1.0e-12) :
    if np.abs(a-b) < tol :
        return True
    return False


class TestExternalSource(unittest.TestCase) :

    def setUp(self) :
        #Callow.initialize(sys.argv)
        pass
    
    def test_externalsource(self) :
        db = InputDB.Create()
        db.put_str("quad_type", "gl")
        db.put_int("quad_number_polar_octant", 4)
        q = QuadratureFactory.build(db, 1)
        mesh = Mesh1D.Create([10],[0.0,10.0],[0])
        s = ConstantSource(1, mesh, 1.0, q)
        self.assertTrue(soft_equiv(s.source(0, 0),    1.0))
        self.assertTrue(soft_equiv(s.source(0, 0, 0), 0.5))

if __name__ == '__main__':
    unittest.main()
