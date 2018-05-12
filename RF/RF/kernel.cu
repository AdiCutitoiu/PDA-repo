
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <cstdlib>

__global__ void RoyFloydStep(int k, int * m, int noNodes)
{
  int& thisElem = m[threadIdx.x * noNodes + threadIdx.y];

  int otherFirstElem  = m[threadIdx.x * noNodes + k];
  int otherSecondElem = m[k * noNodes + threadIdx.y];

  if(thisElem > otherFirstElem + otherSecondElem)
    thisElem = otherFirstElem + otherSecondElem;
}

int main()
{
  int m[][5] = {
    { 0, 3, 9, 8, 3 },
    { 5, 0, 1, 4, 2 },
    { 6, 6, 0, 4, 5 },
    { 2, 9, 2, 0, 7 },
    { 7, 9, 3, 2, 0 }
  };

  int * matrix;
  cudaMalloc(&matrix, _countof(m) * _countof(m) * sizeof(int));
  cudaMemcpy(matrix, m, _countof(m) * _countof(m) * sizeof(int), cudaMemcpyHostToDevice);

  dim3 threadsPerBlock(_countof(m), _countof(m));
  for (int k = 0; k < _countof(m); k++)
  {
    RoyFloydStep<<<1, threadsPerBlock>>>(k, matrix, _countof(m));
  }

  cudaMemcpy(m, matrix, _countof(m) * _countof(m) * sizeof(int), cudaMemcpyDeviceToHost);
  cudaFree(matrix);

  for (const auto & line : m)
  {
    for (auto elem : line)
    {
      std::cout << elem << ' ';
    }
    std::cout << '\n';
  }

  return 0;
}
