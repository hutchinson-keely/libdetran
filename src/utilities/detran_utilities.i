//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   detran_utilities.i
 *  @author Jeremy Roberts
 *  @brief  Python interface for detran utilities.
 */
//----------------------------------------------------------------------------//

%module(package="detran") utilities
%{
#include <stddef.h>
#include "utilities/utilities_export.hh"
#include "utilities/Definitions.hh"
#include "utilities/DBC.hh"
#include "utilities/InputDB.hh"
#include "utilities/MathUtilities.hh"
#include "utilities/SP.hh"
%}

// Somewhat nicely-formatted documentation from C++ source
%feature("autodoc", "3");

// Hide templates from SWIG
%inline
{
#define UTILITIES_EXPORT
#define UTILITIES_TEMPLATE_EXPORT(...)
#define UTILITIES_INSTANTIATE_EXPORT(...)
}

// STL
%include std_container.i
%include std_map.i
%include std_string.i
%include std_vector.i

// Smart pointers can be included as follows:
//   %shared_ptr(Base)
//   %shared_ptr(Derived<int>)
//   %shared_ptr(Derived<double>)
// Any shared_ptr(Foo) declaration *must* come before inclusion of Foo.hh.  
// In addition, any Base class must be declared before its derivative classes.
#define SWIG_SHARED_PTR_NAMESPACE detran_utilities
#define shared_ptr SP
%include <boost_shared_ptr.i>

// STD Vectors
%include "Definitions.hh"
namespace std
{
  %template(vec_int)      vector<int>;
  %template(vec2_int)     vector<vector<int> >;
  %template(vec3_int)     vector<vector<vector<int> > >;
  %template(vec_dbl)      vector<double>;
  %template(vec2_dbl)     vector<vector<double> >;
  %template(vec3_dbl)     vector<vector<vector<double> > >;
  %template(vec_size_t)   vector<unsigned int>;
  %template(vec2_size_t)  vector<vector<unsigned int> >;
  %template(vec3_size_t)  vector<vector<vector<unsigned int> > >;
}

// Parameter database
%shared_ptr(detran_utilities::InputDB)
%include "InputDB.hh"
namespace detran_utilities
{
%template(get_int)      InputDB::get<int>;
%template(get_dbl)      InputDB::get<double>;
%template(get_vec_int)  InputDB::get<vec_int>;
%template(get_vec_dbl)  InputDB::get<vec_dbl>;
%template(get_str)      InputDB::get<std::string>;
%template(get_spdb)     InputDB::get<InputDB::SP_input>;
%template(put_int)      InputDB::put<int>;
%template(put_dbl)      InputDB::put<double>;
%template(put_vec_int)  InputDB::put<vec_int>;
%template(put_vec_dbl)  InputDB::put<vec_dbl>;
%template(put_str)      InputDB::put<std::string>;
%template(put_spdb)     InputDB::put<InputDB::SP_input>;
}

%include "MathUtilities.hh"

// Anyhere in C/C++ that we need (argc, argv)
%include argcargv.i
%apply (int ARGC, char **ARGV) { (int argc, char *argv[]) } 
%apply (int ARGC, char **ARGV) { (int argc, char **argv) } 

//----------------------------------------------------------------------------//
//              end of detran_utilities.i
//----------------------------------------------------------------------------//
