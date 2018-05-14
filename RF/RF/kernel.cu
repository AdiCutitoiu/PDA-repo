
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
  int h_m[][5] = {
    { 0, 3, 9, 8, 3 },
    { 5, 0, 1, 4, 2 },
    { 6, 6, 0, 4, 5 },
    { 2, 9, 2, 0, 7 },
    { 7, 9, 3, 2, 0 }
  };

  int * d_matrix;
  cudaMalloc(&d_matrix, _countof(h_m) * _countof(h_m) * sizeof(int));
  cudaMemcpy(d_matrix, h_m, _countof(h_m) * _countof(h_m) * sizeof(int), cudaMemcpyHostToDevice);

  dim3 threadsPerBlock(_countof(h_m), _countof(h_m));
  for (int k = 0; k < _countof(h_m); k++)
  {
    RoyFloydStep<<<1, threadsPerBlock>>>(k, d_matrix, _countof(h_m));
  }

  cudaMemcpy(h_m, d_matrix, _countof(h_m) * _countof(h_m) * sizeof(int), cudaMemcpyDeviceToHost);
  cudaFree(d_matrix);

  for (const auto & line : h_m)
  {
    for (auto elem : line)
    {
      std::cout << elem << ' ';
    }
    std::cout << '\n';
  }

  return 0;
}
