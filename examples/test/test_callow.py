import unittest
import sys
import numpy as np
from detran.utilities import *
from detran.callow import *
import detran
import time

def soft_equiv(a, b, tol = 1.0e-12) :
    if np.abs(a-b) < tol :
        return True
    return False

class TestCallow(unittest.TestCase) :

    def setUp(self) :
        Callow.initialize(sys.argv)
        
    def get_matrix(self, n=5, V=2.0) :
        A = Matrix(n, n, 3)
        i = [0, 0]
        j = [0, 1]
        v = [V,-1]
        for k in range(1, n-1) :
          i += [k,k,k]
          j += [k-1,k,k+1]
          v += [-1,V,-1]
        i += [n-1,n-1]
        j += [n-V,n-1]
        v += [-1, V]
#         i = [0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4]
#         j = [0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4]
#         v = [2,-1,-1, 2,-1,-1, 2,-1,-1, 2,-1,-1, 2]
        A.insert(i, j, v)
        A.assemble()
        return A
            
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
        A = self.get_matrix()
        self.assertTrue(A(0, 0) == 2.0)
        I = Matrix(5, 5, 1)
        for i in range(0, 5) :
            I.insert(i, i, 1.0)
        I.assemble()
        self.assertTrue(I(0, 0) == 1.0)
 
    def test_matrix_to_scipy(self) :
        A = self.get_matrix()
        rows = A.get_rows()
        cols = A.get_columns()
        vals = A.get_values()
        try : 
            from scipy.sparse import csr_matrix
            B = csr_matrix((vals, cols, rows), 
                           shape=(A.number_rows(), A.number_columns()))
            B.setdiag([3,3,3,3,3])
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
        A = self.get_matrix()
        A.multiply(x, y)

        # Shell Option 1 -- set a multiply function.  Transpose not available. 
        def fun(x, y) :
            A = self.get_matrix()
            A.multiply(x, y)
        #print help(MatrixShellFunction)
        B = MatrixShellFunction(5, 5)
        B.set_multiply(fun)
        B.multiply(x, z)
        for i in range(0, y.size()) :
           self.assertTrue(soft_equiv(y[i], z[i]))
         
        # Shell Option 2 -- Inherit from MatrixShell class and 
        #   provide implementation for multiply and/or transpose.
        class MyShell(PyMatrixShell):
            def __init__(self, A) :
                super(MyShell, self).__init__(A.number_rows(), A.number_columns())
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
        
        # test solver with regular matrix and a shell matrix
        A = self.get_matrix()
        class MyShell(PyMatrixShell):
            def __init__(self, A) :
                super(MyShell, self).__init__(A.number_rows(), A.number_columns())
                self.A = A
            def multiply(self, x, y) :
                self.A.multiply(x, y) 
            def multiply_transpose(self, x, y) :
                self.A.multiply_transpose(x, y)                
        C = MyShell(A)                
        
        b = Vector(5, 1.0)
        x = Vector(5, 0.0)
        y = Vector(5, 0.0)      
                
        db = InputDB.Create()
        db.put_str("linear_solver_type", "gmres")
        #db.put_int("linear_solver_monitor_level", 2)
        solver = LinearSolver.Create(db)
        solver.set_operator(A)
        solver.solve(b, x)
        A.multiply(x, y)
        for i in range(0, y.size()) :
           self.assertTrue(soft_equiv(y[i], 1.0))
        print solver.number_iterations()
 
        solver.set_operator(C)
        y.set(0.0)
        solver.solve(b, y)
        for i in range(0, y.size()) :
           self.assertTrue(soft_equiv(y[i], x[i]))        
        res = solver.residual_norms()
        print np.asarray(res)
        
    def test_solver_performance(self) :
        n = 200
        A = self.get_matrix(n, 3.0)
        b = Vector(n, 1.0)
        x = Vector(n, 0.0)  
                
        types = ["richardson", "richardson", "jacobi", "gauss-seidel", \
                 "gauss-seidel", "mr1", "mr1", "gmres", "gmres"]
        pcs   = ["", "jacobi", "", "","","", "ilu0", "", "ilu0"]
        sor   = [1.0, 1.0, 1.0, 1.0, 1.16, 1.0, 1.0, 1.0, 1.0]
        db = InputDB.Create()
        db.put_int("linear_solver_maxit", 1000)
        db.put_int("linear_solver_monitor_level", 1)
        db.put_dbl("linear_solver_rtol", 1.0e-9)
        num_iters = []
        nrm_resid = []
        times = []
        for i in range(0, len(types)) :
            x.set(0.0)
            db.put_str("linear_solver_type", types[i])
            db.put_str("pc_type", pcs[i])
            db.put_dbl("linear_solver_relaxation", sor[i])
            solver = LinearSolver.Create(db)
            solver.set_operator(A)
            solver.set_preconditioner()
            t = time.time()
            solver.solve(b, x)
            times.append(time.time()-t)
            num_iters.append(solver.number_iterations())
            nrm_resid.append(np.asarray(solver.residual_norms()))
        print num_iters
        print times
        print np.asarray(times) / np.asarray(num_iters)
        try :
            import matplotlib.pyplot as plt
            plt.semilogy(range(num_iters[2]+1), nrm_resid[2], 'k-o',  \
                         range(num_iters[3]+1), nrm_resid[3], 'b->',  \
                         range(num_iters[4]+1), nrm_resid[4], 'b--^', \
                         range(num_iters[5]+1), nrm_resid[5], 'r-v',  \
                         range(num_iters[6]+1), nrm_resid[6], 'r--<', \
                         range(num_iters[7]+1), nrm_resid[7], 'g-*',  \
                         range(num_iters[8]+1), nrm_resid[8], 'g--h')
            plt.legend(['jacobi','g-s','sor(1.16)','mr1','mr1+ilu0','gmres','gmres+ilu0'])
            plt.xlabel('iteration')
            plt.ylabel('residual norm')
            plt.grid(True)
            plt.show()
        except :
            print "no matplotlib"
            pass
        
    def test_pcmatrix(self) :
        
        x = Vector(5, 1.0)
        y = Vector(5, 0.0)
        z = Vector(5, 0.0)
        A = self.get_matrix()   
        
        class MyShell(PyMatrixShell):
            def __init__(self, A) :
                super(MyShell, self).__init__(A.number_rows(), A.number_columns())
                self.A = A
            def multiply(self, x, y) :
                self.A.multiply(x, y) 
            def multiply_transpose(self, x, y) :
                self.A.multiply_transpose(x, y)  
        B = MyShell(A)
        
        # we can use a python class in detran...neat.
        P = PCMatrix(B)
        P.apply(x, y)
        A.multiply(x, z)
        for i in range(0, y.size()) :
           self.assertTrue(soft_equiv(y[i], z[i]))  
   
if __name__ == '__main__':
    unittest.main()
