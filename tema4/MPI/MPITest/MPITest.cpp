// MPITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../MPILib/Handler.h"
#include "../MPILib/Communicator.h"
#include "../MPILib/Process.h"

#include <random>

using namespace MPI;

void Leader(Session & aSession)
{
  random_device x;

  auto comm = aSession.GetWorldCommunicator();

  int number = (x() % comm.GetSize());
  int rank = comm.GetCurrentProcess().GetRank();

  int leaderNumber = number;
  int leaderRank = rank;
  auto procs = comm.GetAllProcesses();
  for (auto & proc : procs)
  {
    if (proc.GetRank() != rank)
    {
      int items[2] = { number, rank };
      comm.Send(proc, items, _countof(items));
    }
  }

  for (auto & proc : procs)
  {
    if (proc.GetRank() != rank)
    {
      auto received = comm.Receive<int>(proc, 2);
      int procNumber = received.value()[0];
      int procRank = received.value()[1];

      if (procNumber > leaderNumber)
      {
        leaderNumber = procNumber;
        leaderRank = procRank;
      }
      else if (procNumber == leaderNumber)
      {
        leaderRank = max(leaderRank, procRank);
      }
    }
  }

  printf("Proc %d generated %d. Leader is %d with %d", rank, number, leaderRank, leaderNumber);
}

int kNumbers = 50'000;
void MasterSum(Session & aSession)
{
  auto comm = aSession.GetWorldCommunicator();
  auto slaveProcs = comm.GetSlaveProcesses();

  vector<int> numbers(kNumbers);

  std::generate(numbers.begin(), numbers.end(), [n = 1]() mutable { return n++; });

  int subvecSize = numbers.size() / slaveProcs.size();
  int remainder = numbers.size() % slaveProcs.size();

  for (size_t i = 0; i < slaveProcs.size(); i++)
  {
    if (i == slaveProcs.size() - 1)
    {
      comm.Send(slaveProcs[i], &numbers[i * subvecSize], subvecSize + remainder);
    }
    else
    {
      comm.Send(slaveProcs[i], &numbers[i * subvecSize], subvecSize);
    }
  }

  int totalSum = std::accumulate(slaveProcs.begin(), slaveProcs.end(), 0, 
                                 [&](auto aPrevValue, Process & aProcess) {
                                   return aPrevValue + comm.Receive<int>(aProcess).value();
                                 });
  
  printf("MASTER: total sum is %d", totalSum);
}

void SlaveSum(Session & aSession)
{
  auto comm = aSession.GetWorldCommunicator();
  auto slaveProcs = comm.GetSlaveProcesses();

  int subvecSize = kNumbers / slaveProcs.size();
  int remainder = kNumbers % slaveProcs.size();

  optional<vector<int>> result;
  if (comm.GetCurrentProcess().GetRank() == comm.GetSize() - 1)
  {
    result = comm.Receive<int>(comm.GetMasterProcess(), subvecSize + remainder);
  }
  else
  {
    result = comm.Receive<int>(comm.GetMasterProcess(), subvecSize);
  }

  auto sum = std::accumulate(result.value().begin(), result.value().end(), 0);
  printf("Process %d computed the sum: %d\n", comm.GetCurrentProcess().GetRank(), sum);

  comm.Send(comm.GetMasterProcess(), &sum, 1);
}

int main(int argc, char ** argv)
{
  

  Handler::Start(
    argc,
    argv,
    MasterSum,
    SlaveSum
  );

  return 0;
}

