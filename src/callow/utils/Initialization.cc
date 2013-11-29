//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file   Initialization.cc
 *  @brief  Initialization member definitions
 *  @note   Copyright (C) 2012-2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#include "callow/utils/Initialization.hh"

namespace callow
{

//----------------------------------------------------------------------------//
void Callow::initialize()
{
  if (get_instance().initialized) return;
  int argc = 0;
  char** argv = NULL;
  initialize(argc, argv);
}

//----------------------------------------------------------------------------//
void Callow::initialize(int argc, char *argv[])
{
  if (get_instance().initialized) return;
#ifdef CALLOW_ENABLE_PETSC
  PetscInitialize(&argc, &argv, PETSC_NULL, PETSC_NULL);
#endif
#ifdef CALLOW_ENABLE_SLEPC
  SlepcInitialize(&argc, &argv, PETSC_NULL, PETSC_NULL);
#endif
  get_instance().initialized = true;
}

//----------------------------------------------------------------------------//
void Callow::finalize()
{
  if (!get_instance().initialized) return;
#ifdef CALLOW_ENABLE_SLEPC
  SlepcFinalize();
#endif
#ifdef CALLOW_ENABLE_PETSC
  PetscFinalize();
#endif
  get_instance().initialized = false;
}

//----------------------------------------------------------------------------//
Callow::Callow()
  : initialized(false)
{
  /* ... */
}

//----------------------------------------------------------------------------//
Callow::~Callow()
{
  finalize();
}

//----------------------------------------------------------------------------//
Callow& Callow::get_instance()
{
  static Callow instance;
  return instance;
}

} // end namespace callow

//----------------------------------------------------------------------------//
void callow_initialize(int argc, char *argv[])
{
#ifdef CALLOW_ENABLE_PETSC
  PetscInitialize(&argc, &argv, PETSC_NULL, PETSC_NULL);
#endif
#ifdef CALLOW_ENABLE_SLEPC
  SlepcInitialize(&argc, &argv, PETSC_NULL, PETSC_NULL);
#endif
}

//----------------------------------------------------------------------------//
void callow_finalize()
{
#ifdef CALLOW_ENABLE_SLEPC
  SlepcFinalize();
#endif
#ifdef CALLOW_ENABLE_PETSC
  PetscFinalize();
#endif
}


//----------------------------------------------------------------------------//
//              end of file Initialization.cc
//----------------------------------------------------------------------------//

