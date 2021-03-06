// MPITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <random>
#include <algorithm>

constexpr int kInf = 99999;
constexpr int kMaster = 0;

int GetRank()
{
  int rank = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  return rank;
}

int main(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);

  int m[][4] = { 
    { 0,    5,    kInf, 10 },
    { kInf, 0,    3,    kInf },
    { kInf, kInf, 0,    1 },
    { 1   , kInf, kInf, 0 }
  };

  auto procRank = GetRank();
  for (int k = 0; k < _countof(m); k++)
  {
    for (int j = 0; j < _countof(m); j++)
    {
      m[procRank][j] = min(m[procRank][k] + m[k][j], m[procRank][j]);
    }

    MPI_Allgather(m[procRank], _countof(m[procRank]), MPI_INT, m, _countof(m[procRank]), MPI_INT, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  if (procRank == kMaster)
  {
    for (auto & line : m)
    {
      for (auto elem : line)
      {
        cout << elem << ' ';
      }
      cout << '\n';
    }
    cout << '\n';
  }

  return 0;
}
