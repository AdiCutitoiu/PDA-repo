#include "stdafx.h"
#include "Process.h"
#include "MPIDataType.h"

namespace
{
  constexpr int kMaster = 0;
}

using namespace MPI;

Process::Process(const MPI_Comm & aCommunicator, int aRank)
  : mCommunicator { aCommunicator }
  , mRank {aRank}
{
}

bool MPI::Process::IsMaster() const
{
  return kMaster == mRank;
}

bool MPI::Process::IsCurrentProcess() const
{
  int rank = 0;

  MPI_Comm_rank(mCommunicator, &rank);

  return mRank == rank;
}

int MPI::Process::GetRank() const
{
  return mRank;
}
