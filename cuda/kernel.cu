#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "thrust\reduce.h"

#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include <iostream>

using namespace std;

#ifndef __CUDACC__
#define __CUDACC__
#endif // !__CUDACC__

#ifndef CORES_LENGTH
#define CORES_LENGTH 2000
#endif // !CORES_LENGTH

__device__ double Calculate(int x);

__global__ void ThreadsToCalc(double *dest);

__shared__ double sdata[CORES_LENGTH];

__global__ void SwapToMin(double *g_idata, double *g_odata);

int main()
{
    // compute the function values
    const int blockSize = 256;
    double *calculated_value;
    int nBlocks = CORES_LENGTH / blockSize + 1;

    cudaMalloc(&calculated_value, CORES_LENGTH * sizeof(double));
    ThreadsToCalc<<<nBlocks, blockSize>>>(calculated_value);
    cudaDeviceSynchronize();

    double *memory_allocated = (double *)malloc(CORES_LENGTH * sizeof(double));
    cout << "allocated memory size: " << memory_allocated << endl;

    cudaMemcpy(memory_allocated, calculated_value, CORES_LENGTH * sizeof(double), cudaMemcpyDeviceToHost);

    for (int i = 0; i < 20; i++)
    {
        cout << "allocated mem for " << i << "iteration is: " << memory_allocated[i] << endl;
    }

    double *ouput_size;

    cudaMalloc(&ouput_size, CORES_LENGTH * sizeof(double));
    SwapToMin<<<nBlocks, blockSize>>>(calculated_value, ouput_size);
    cudaDeviceSynchronize();

    double *res_min = (double *)malloc(CORES_LENGTH * sizeof(double));

    cudaMemcpy(res_min, ouput_size, CORES_LENGTH * sizeof(double), cudaMemcpyDeviceToHost);
    cout << "min is: " << res_min[0] << endl;
}

__device__ double Calculate(int x)
{
    return sin(5 * pow(x, 3)) + cos(7 * pow(x, 4)) * x;
}

__global__ void ThreadsToCalc(double *dest)
{
    int start = blockDim.x * (blockIdx.x);
    int index = start + threadIdx.x > CORES_LENGTH
                    ? start + threadIdx.x
                    : CORES_LENGTH;
    dest[start + threadIdx.x] = Calculate(start + threadIdx.x);
}

__global__ void SwapToMin(double *g_idata, double *g_odata)
{
    extern __shared__ double sdata[];
    unsigned int thread_id = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    sdata[thread_id] = g_idata[i];
    __syncthreads();
    for (unsigned int st = 1; st < blockDim.x; st *= 4)
    {
        if (thread_id % (2 * st) == 0)
        {
            if (sdata[thread_id] > sdata[thread_id + st])
                sdata[thread_id] = sdata[thread_id + st];
        }
        __syncthreads();
    }
    if (thread_id == 0)
        g_odata[blockIdx.x] = sdata[0];
}
