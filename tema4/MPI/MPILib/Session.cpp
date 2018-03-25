#include "stdafx.h"
#include "Session.h"
#include "mpi.h"

using namespace MPI;

Session::operator bool() const
{
  return MPI_SUCCESS == mErrorCode;
}

int Session::GetErrorCode() const
{
  return mErrorCode;
}

Communicator Session::GetWorldCommunicator() const
{
  return Communicator(MPI_COMM_WORLD);
}

Session::Session(int & aArgc, char **& aArgv)
  : mErrorCode { MPI_Init(&aArgc, &aArgv)  }
{
}

Session::~Session()
{
  
}

void Session::ExecuteProc(function<void(Session&)> aProc)
{
  aProc(*this);
}

