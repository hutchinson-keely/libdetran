//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   Initialization.hh
 *  @brief  Initialization for callow third-party libraries
 *  @note   Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#ifndef callow_INITIALIZATION_HH_
#define callow_INITIALIZATION_HH_

#include "callow/callow_config.hh"

namespace callow
{

class Callow
{

public:

  static void initialize();
  static void initialize(int argc, char *argv[]);
  static void finalize();

private :

  Callow();
  ~Callow();
  static Callow& get_instance();
  bool initialized;

};

} // end namespace callow

/// Initialize external packages, if enabled
void callow_initialize(int argc, char *argv[]);

/// Finalize external packages, if enabled
void callow_finalize();


#endif /* callow_INITIALIZATION_HH_ */

//----------------------------------------------------------------------------//
//              end of file Initialization.hh
//----------------------------------------------------------------------------//
