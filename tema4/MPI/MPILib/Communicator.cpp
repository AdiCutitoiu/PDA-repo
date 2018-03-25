#include "stdafx.h"
#include "Communicator.h"

namespace
{
  constexpr int kMaster = 0;
}

using namespace MPI;

Communicator::Communicator(MPI_Comm aCommunicator)
  : mCommunicator{ aCommunicator }
{
}

void Communicator::Abort(int aErrorCode) const
{
  MPI_Abort(mCommunicator, aErrorCode);
}

Process Communicator::GetCurrentProcess() const
{
  int rank = 0;

  MPI_Comm_rank(mCommunicator, &rank);

  return Process(mCommunicator, rank);
}

Process Communicator::GetMasterProcess() const
{
  return Process(mCommunicator, kMaster);
}

vector<Process> MPI::Communicator::GetSlaveProcesses() const
{
  vector<Process> result;

  for (int rank = 1; rank < GetSize(); rank++)
  {
    result.emplace_back(mCommunicator, rank);
  }

  return result;
}

vector<Process> MPI::Communicator::GetAllProcesses() const
{
  vector<Process> result;

  for (int rank = 0; rank < GetSize(); rank++)
  {
    result.emplace_back(mCommunicator, rank);
  }

  return result;
}

int Communicator::GetSize() const
{
  int size = 0;

  MPI_Comm_size(mCommunicator, &size);

  return size;
}
