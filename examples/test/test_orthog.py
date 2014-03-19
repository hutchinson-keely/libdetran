import unittest
import sys
import numpy as np
from detran.utilities import *
from detran.orthog import *
import detran
import time

def soft_equiv(a, b, tol = 1.0e-12) :
    if np.abs(a-b) < tol :
        return True
    return False


class TestOrthog(unittest.TestCase) :

    def setUp(self) :
        #Callow.initialize(sys.argv)
        pass
    
    def test_orthog(self) :
        P = OrthogonalBasisParameters()
        P.order = 1
        P.size = 10
        B = OrthogonalBasis.Create("dlp", P)
        print detran.callow.mat_asarray(B.basis())
        
        
if __name__ == '__main__':
    unittest.main()
