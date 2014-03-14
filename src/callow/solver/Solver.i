//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   Solver.i
 *  @author Jeremy Roberts
 *  @brief  Python interface for callow solvers
 */
//----------------------------------------------------------------------------//

%shared_ptr(callow::LinearSolver)
%shared_ptr(callow::EigenSolver)
 
%include "LinearSolver.hh"
%include "EigenSolver.hh"
 