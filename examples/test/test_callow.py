import unittest
import sys
import numpy as np
from detran.callow import *

class TestCallow(unittest.TestCase) :

    def setUp(self) :
        Callow.initialize(sys.argv)
        
    def test_vector(self) :
        V1 = Vector.Create(10, 1.00)
        V2 = Vector(10, 2.00)
        
        # Basic vector accessors
        self.assertTrue(V1[0] == 1.00)
        self.assertTrue(V2[0] == 2.00)
        self.assertTrue(V1.size() == 10)
        
        # Vector operations
        v = V1.norm(0)
        self.assertTrue(v == 10.0)
        V1.add(V2)
        v = V1.norm(0)
        self.assertTrue(v == 30.0)

    def test_matrix(self) :
        A = Matrix(5, 5, 3)
        i = [0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4]
        j = [0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4]
        v = [2,-1,-1, 2,-1,-1, 2,-1,-1, 2,-1,-1, 2]
        A.insert(i, j, v)
        A.assemble()
        self.assertTrue(A(0, 0) == 2.0)
        
        I = Matrix(5, 5, 1)
        for i in range(0, 5) :
            I.insert(i, i, 1.0)
        I.assemble()
        self.assertTrue(I(0, 0) == 1.0)
        
    def test_matrixdense(self) :    
        A = MatrixDense(5, 5)
        v = np.array(range(0, 25), 'f')
        A.insert(v)
        A.display()
        
if __name__ == '__main__':
    unittest.main()
