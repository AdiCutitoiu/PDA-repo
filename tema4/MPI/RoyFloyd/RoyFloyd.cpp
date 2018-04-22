// MPITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../MPILib/Handler.h"
#include "../MPILib/Communicator.h"
#include "../MPILib/Process.h"

#include <random>
#include <algorithm>

using namespace MPI;

int kNodes = 5;
int kNumbers = 50'000;
void Master(Session & aSession)
{
  vector<vector<int>> m = {
    { 0, 3, 9, 8, 3 },
    //{ 5, 0, 1, 4, 2 },
    //{ 6, 6, 0, 4, 5 },
    //{ 2, 9, 2, 0, 7 },
    //{ 7, 9, 3, 2, 0 }
  };
  cout << 'x';
  auto comm = aSession.GetWorldCommunicator();
  //
  //for (int k = 0; k < kNodes; k++) {
    auto newM = comm.AllGather(m, kNodes);
  //  if (!newM)
  //    return;
  //
  //  m = move(newM.value());
  //}
}

void Slave(Session & aSession)
{
  auto comm = aSession.GetWorldCommunicator();

  vector<vector<int>> m;
  const int kLine = 0;
  //for (int k = 0; k < kNodes; k++) {

    //vector<vector<int>> m;
    //
    //for(int j = 0; j < kNodes; j++) 
      auto line = comm.Receive<int>(comm.GetMasterProcess(), kNodes);
    //  if (!line)
    //    return;
    //
    //  m.emplace_back(move(line.value()));
    //}
    //
    //for (auto line : m) {
    //  for (auto x : line) {
    //    cout << x << ' ';
    //  }
    //  cout << '\n';
    //}
    //cout << '\n';
    //cout << '\n';

    //auto lineIndex = comm.GetCurrentProcess().GetRank() - 1;
    //for (int j = 0; j < kNodes; j++) {
    //  m[lineIndex][j] = std::min(m[lineIndex][j], m[lineIndex][k] + m[k][lineIndex]);
    //}
    //
    //comm.Send(comm.GetMasterProcess(), m[lineIndex]);
  //}
}

int main(int argc, char ** argv)
{
  Handler::Start(
    argc,
    argv,
    Master,
    Slave
  );

  return 0;
}

