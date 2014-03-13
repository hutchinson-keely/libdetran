import unittest
import sys
import numpy as np
from detran.utilities import *
from detran.callow import *
import detran

def soft_equiv(a, b, tol = 1.0e-12) :
    if np.abs(a-b) < tol :
        return True
    return False

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
        AA=mat_asarray(A) # note, stored as col-major in detran; row major in np
        for i in range(0, 5) :
            for j in range(0, 5) :
                k = i + 5*j
                self.assertTrue(soft_equiv(AA[j][i], v[k]))
        
    def test_matrixshell(self) :
        x = Vector(5, 1.0)
        y = Vector(5, 0.0)
        z = Vector(5, 0.0)
        
        A = Matrix(5, 5, 3)
        i = [0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4]
        j = [0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4]
        v = [2,-1,-1, 2,-1,-1, 2,-1,-1, 2,-1,-1, 2]
        A.insert(i, j, v)
        A.assemble()

        class MyShell(PyMatrixShell) :
            def __init__(self, n, m, A) :
                PyMatrixShell.__init__(self, n, m)
                self.A = A
                self.set_multiply(self.do_multiply)
            def do_multiply(self, xx, yy) :
                self.A.multiply(xx, yy) 
                
        B = MyShell(5, 5, A)
        A.multiply(x, y)
        B.multiply(x, z)
        for i in range(0, y.size()) :
           self.assertTrue(soft_equiv(y[i], z[i]))
        
    def test_solver(self) : 
        A = Matrix(5, 5, 3)
        i = [0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4]
        j = [0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4]
        v = [2,-1,-1, 2,-1,-1, 2,-1,-1, 2,-1,-1, 2]
        A.insert(i, j, v)
        A.assemble()
        b = Vector(5, 1.0)
        x = Vector(5, 0.0)
        y = Vector(5, 0.0)      
        db = InputDB.Create()
        db.put_str("linear_solver_type", "gmres")
        solver = LinearSolverCreator.Create(db)
        solver.set_operators(A)
        solver.solve(b, x)
        A.multiply(x, y)
        for i in range(0, y.size()) :
           self.assertTrue(soft_equiv(y[i], 1.0))
           
    def test_pcmatrix(self) :
        
        x = Vector(5, 1.0)
        y = Vector(5, 0.0)
        z = Vector(5, 0.0)
        A = Matrix(5, 5, 3)
        i = [0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4]
        j = [0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4]
        v = [2,-1,-1, 2,-1,-1, 2,-1,-1, 2,-1,-1, 2]
        A.insert(i, j, v)
        A.assemble()      
        
        class MyShell(PyMatrixShell) :
            def __init__(self, n, m, A) :
                PyMatrixShell.__init__(self, n, m)
                self.A = A
                self.set_multiply(self.do_multiply)
            def do_multiply(self, xx, yy) :
                self.A.multiply(xx, yy) 
        B = MyShell(5, 5, A)
        
        # we can use a python class in detran...neat.
        P = PCMatrix(B)
        P.apply(x, y)
        A.multiply(x, z)
        for i in range(0, y.size()) :
           self.assertTrue(soft_equiv(y[i], z[i]))  
    
if __name__ == '__main__':
    unittest.main()
