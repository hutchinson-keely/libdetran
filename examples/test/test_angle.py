import unittest
import sys
import numpy as np
from detran.utilities import *
from detran.angle import *
import detran
import time

def soft_equiv(a, b, tol = 1.0e-12) :
    if np.abs(a-b) < tol :
        return True
    return False


class TestAngle(unittest.TestCase) :

    def setUp(self) :
        #Callow.initialize(sys.argv)
        pass
    
    def test_quadrature(self) :
        db = InputDB.Create()
        db.put_str("quad_type", "gl")
        db.put_int("quad_number_polar_octant", 4)
        q = QuadratureFactory.build(db, 1)
        q.display()
        
if __name__ == '__main__':
    unittest.main()
