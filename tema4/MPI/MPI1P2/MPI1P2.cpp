// MPI1P2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../MPILib/Handler.h"
#include "../MPILib/Communicator.h"
#include "../MPILib/Process.h"

using namespace MPI;

const int maxNumber = 50'000'000;

void MasterProc(Session & aSession)
{
  vector<int> vec;

  auto slaveProcs = aSession.GetWorldCommunicator().GetSlaveProcesses();
  int step = maxNumber / slaveProcs.size();
  int remainder = maxNumber % slaveProcs.size();

  for (int i = 0; i < static_cast<int>(slaveProcs.size()); i++)
  {
    aSession.GetWorldCommunicator().Send(slaveProcs[i], &i, 1);
  }

  int found = 0;
  //for (auto & proc : slaveProcs)
  //{
  //  auto received = aSession.GetWorldCommunicator().Receive<int>(proc);
  //  if (!received)
  //    continue;
  //
  //  found = found || received.value();
  //  if (found)
  //    break;
  //}
}

void SlaveProc(Session & aSession)
{
  vector<int> vec;
  
  auto comm = aSession.GetWorldCommunicator();
  
  int step = maxNumber / comm.GetSlaveProcesses().size();
  int remainder = maxNumber % comm.GetSlaveProcesses().size();
  
  optional<int> res;
  if (comm.GetCurrentProcess().GetRank() == maxNumber - 1)
  {
    res = comm.Receive<int>(comm.GetMasterProcess());
  }
  else
  {
    res = comm.Receive<int>(comm.GetMasterProcess());
  }

  int found = 1;
  comm.Send(comm.GetMasterProcess(), &found, 1);
  
  //if (!res)
  //{
  //  return;
  //}
  //
  //auto numbers = move(res.value());
  //
  //int found = 0;
  //for (size_t i = 0; i < numbers.size() && !found; i++)
  //{
  //  found = numbers[i] == 5;
  //}
  //
  //comm.Send(comm.GetMasterProcess(), &found, 1);
}

int main(int argc, char ** argv)
{
  auto proc = [&](Session & aSession) {

  };

  Handler::Start(
    argc,
    argv,
    MasterProc,
    SlaveProc
  );


  return 0;
}
