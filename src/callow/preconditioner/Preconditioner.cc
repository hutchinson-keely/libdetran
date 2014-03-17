//----------------------------------*-C++-*-----------------------------------//
/**
 *  @file  Preconditioner.cc
 *  @brief Preconditioner member definitions
 *  @note  Copyright (C) 2013 Jeremy Roberts
 */
//----------------------------------------------------------------------------//

#include "Preconditioner.hh"
#include "PCIdentity.hh"
#include "PCILU0.hh"
#include "PCJacobi.hh"
#include "PCMatrix.hh"

namespace callow
{

void InitPreconditionerFactory()
{
  REGISTER_CLASS(Preconditioner, PCIdentity,       "identity")
  REGISTER_CLASS(Preconditioner, PCJacobi,         "jacobi")
  REGISTER_CLASS(Preconditioner, PCILU0,           "ilu0")
  REGISTER_CLASS(Preconditioner, PCMatrix,         "matrix")
}

//----------------------------------------------------------------------------//
Preconditioner::SP_preconditioner
Preconditioner::Create(SP_matrix A, SP_db db)
{
  InitPreconditionerFactory();
  // get key
  std::string key = "ilu0";
  if (db)
  {
    if (db->check("pc_type"))
      key = db->get<std::string>("pc_type");
  }
  else
  {
    db = new detran_utilities::InputDB();
    db->put<std::string>("pc_type", key);
  }
  return (Factory_T::Instance().GetCreateFunction(key))(A, db);
}

//----------------------------------------------------------------------------//
Preconditioner::Preconditioner(const std::string &name, SP_db db)
  : d_name(name)
  , d_db(db)
  , d_petsc_pc(NULL)
  , d_slepc_st(NULL)
  , d_size(0)
{
  /* ... */
}

//----------------------------------------------------------------------------//
void Preconditioner::display(const std::string &name)
{
  Assert(d_size > 0);
  MatrixShellPC M(this, d_size);
  M.compute_explicit(name);
}


#ifdef DETRAN_ENABLE_PETSC

//----------------------------------------------------------------------------//
void Preconditioner::set_petsc_pc(PC pc)
{
  // save the pc object
  d_petsc_pc = pc;

  PetscErrorCode ierr;
  // set the shell preconditioner
  ierr = PCSetType(pc, PCSHELL);
  // set the PC context
  ierr = PCShellSetContext(d_petsc_pc, this);
  Insist(!ierr, "Error setting shell preconditioner context.");
  // set the PC operator
  ierr = PCShellSetApply(d_petsc_pc, pc_apply_wrapper);
  Insist(!ierr, "Error setting shell preconditioner operator.");
  // set the PC name for good measure
  ierr = PCShellSetName(d_petsc_pc, d_name.c_str());
  Insist(!ierr, "Error within-group preconditioner name.");

  Ensure(!ierr);
}

//----------------------------------------------------------------------------//
PetscErrorCode pc_apply_wrapper(PC pc, Vec b, Vec x)
{
  // get the context and cast
  PetscErrorCode ierr;
  void *context;
  ierr = PCShellGetContext(pc, &context); CHKERRQ(ierr);
  Preconditioner *foo = (Preconditioner *) context;
  // wrap the petsc vectors
  Vector B(b);
  Vector X(x);
  // call the actual apply operator.
  foo->apply(B, X);
  return ierr;
}

#else

inline void Preconditioner::set_petsc_pc(PC pc){}
inline PetscErrorCode pc_apply_wrapper(PC pc, Vec b, Vec x){return 0;}

#endif

#ifdef DETRAN_ENABLE_SLEPC

//----------------------------------------------------------------------------//
void Preconditioner::set_slepc_st(ST st)
{
  std::cout << "SETTING ST WRAPPER!!!" << std::endl;

  // save the pc object
  d_slepc_st = st;

  PetscErrorCode ierr;
  // set the shell preconditioner
  //ierr = STSetType(st, STSHELL);
  // set the PC context
  ierr = STShellSetContext(d_slepc_st, this);
  Insist(!ierr, "Error setting shell spectral transform context.");
  // set the PC operator
  ierr = STShellSetApply(d_slepc_st, st_apply_wrapper);
  Insist(!ierr, "Error setting shell spectral transform operator.");

  Ensure(!ierr);
}

//----------------------------------------------------------------------------//
PetscErrorCode st_apply_wrapper(ST st, Vec b, Vec x)
{
  std::cout << "APPLYING ST WRAPPER!!!" << std::endl;
  // get the context and cast
  PetscErrorCode ierr;
  void *context;
  ierr = STShellGetContext(st, &context); CHKERRQ(ierr);
  Preconditioner *foo = (Preconditioner *) context;
  // wrap the petsc vectors
  Vector B(b);
  Vector X(x);
  // call the actual apply operator.
  foo->apply(B, X);
  return ierr;
}

#else

inline void Preconditioner::set_slepc_st(ST st){}
inline PetscErrorCode st_apply_wrapper(ST st, Vec b, Vec x){return 0;}

#endif

} // end namespace callow

//----------------------------------------------------------------------------//
//              end of file Preconditioner.hh
//----------------------------------------------------------------------------//
