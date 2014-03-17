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
 
    def test_matrix_to_scipy(self) :
        A = Matrix(5, 5, 3)
        i = [0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4]
        j = [0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4]
        v = [2, -1, -1, 2, -1, -1, 2, -1, -1, 2, -1, -1, 2]
        A.insert(i, j, v)
        A.assemble()
        self.assertTrue(A(0, 0) == 2.0)
        rows = A.get_rows()
        cols = A.get_columns()
        vals = A.get_values()
        try : 
            from scipy.sparse import csr_matrix
            B = csr_matrix((vals, cols, rows), 
                           shape=(A.number_rows(), A.number_columns()))
            print B
            B.setdiag([3,3,3,3,3])
            A.display()
            del B # all done
            self.assertTrue(A(0, 0) == 3.0)
        except :
            print "scipy not available"
        del rows
        del cols 
        del vals
 
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
        w = Vector(5, 0.0)

        # make reference matrix and result        
        A = Matrix(5, 5, 3)
        i = [0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4]
        j = [0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4]
        v = [2,-1,-1, 2,-1,-1, 2,-1,-1, 2,-1,-1, 2]
        A.insert(i, j, v)
        A.assemble()
        A.multiply(x, y)

        # Shell Option 1 -- set a multiply function.  Transpose not available. 
        def multiply(x, y) :
            A = Matrix(5, 5, 3)
            i = [0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4]
            j = [0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4]
            v = [2,-1,-1, 2,-1,-1, 2,-1,-1, 2,-1,-1, 2]
            A.insert(i, j, v)
            A.assemble()
            A.multiply(x, y)
        B = PyMatrixShell(5, 5)
        B.set_multiply(multiply)
        B.multiply(x, z)
        for i in range(0, y.size()) :
           self.assertTrue(soft_equiv(y[i], z[i]))
        
        # Shell Option 2 -- Inherit from MatrixShell class and 
        #   provide implementation for multiply and/or transpose.
        class MyShell(MatrixShell):
            def __init__(self, A) :
                super(MyShell, self).__init__(None, A.number_rows(), A.number_columns())
                self.A = A
            def multiply(self, x, y) :
                self.A.multiply(x, y) 
            def multiply_transpose(self, x, y) :
                self.A.multiply_transpose(x, y)
        C = MyShell(A)
        C.multiply(x, w)
        for i in range(0, z.size()) :
           self.assertTrue(soft_equiv(z[i], w[i]))
           
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
        solver = LinearSolver.Create(db)
        solver.set_operator(A)
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
