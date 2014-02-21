# src/python/detran/__init__.py
#
# Copyright (C) 2013 Jeremy Roberts <j.alyn.roberts@gmail.com>

import sys
try :
  import numpy as np
except ImportError :
  print "ERROR: Could not import Numpy, which is needed for pydetran."
  raise
  
import utilities
import callow

#from angle import *
 
#from orthog import *
#from geometry import *
#from material import *
#from external_source import *
# from boundary import *
# from transport import *
# from kinetics import *
# from ioutils import *
# from solvers import *
# from postprocess import *

#from pydetranutils import *
 

callow.Callow.initialize(sys.argv)
