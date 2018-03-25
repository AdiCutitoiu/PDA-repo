// MPITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../MPILib/Handler.h"
#include "../MPILib/Communicator.h"
#include "../MPILib/Process.h"

using namespace MPI;

int main(int argc, char ** argv)
{
   
  auto proc = [&](Session & aSession) {
   
    int numprocs, procid, partner, message;

    auto procs = aSession.GetWorldCommunicator().GetAllProcesses();
    numprocs = procs.size();

    procid = aSession.GetWorldCommunicator().GetCurrentProcess().GetRank();
    message = procid;

    auto currentProcess = aSession.GetWorldCommunicator().GetCurrentProcess();
    if (currentProcess.IsMaster())
      printf("MASTER: Number of MPI procs is: %d\n", numprocs);

    partner = numprocs - procid - 1;

    std::cout << procid << ' ' << partner << endl;

    aSession.GetWorldCommunicator().Send(procs[partner], vector<int>{procid});

    auto res = aSession.GetWorldCommunicator().Receive<int>(procs[partner], 1);
    if (res)
      message = res.value().front();

    // print partner info and exit
    printf("Proc %d is partner with %d\n", procid, message);
  };

  Handler::Start(
    argc,
    argv,
    proc,
    proc
  );
  

  return 0;
}

