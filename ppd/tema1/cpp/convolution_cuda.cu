#include "convolution.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace CudaImpl
{

    ConvolutionData readInput(const std::string &filename)
    {
        ConvolutionData data;

        std::ifstream fin(filename);
        if (!fin)
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            exit(1);
        }

        fin >> data.n >> data.m >> data.k;

        // Allocate host (CPU) memory as 1D array
        data.h_F = new int[data.n * data.m];
        data.h_C = new int[data.k * data.k];

        // read matrix F (n x m)
        for (int i = 0; i < data.n; i++)
        {
            for (int j = 0; j < data.m; j++)
            {
                fin >> data.h_F[i * data.m + j]; // 1D indexing
            }
        }

        // read kernel C (k x k)
        for (int i = 0; i < data.k; i++)
        {
            for (int j = 0; j < data.k; j++)
            {
                fin >> data.h_C[i * data.k + j]; // 1d indexing
            }
        }

        fin.close();

        // Allocate device (GPU) memory
        cudaMalloc(&data.d_F, data.n * data.m * sizeof(int));
        cudaMalloc(&data.d_C, data.k * data.k * sizeof(int));

        // Copy data from host to device
        cudaMemcpy(data.d_F, data.h_F, data.n * data.m * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(data.d_C, data.h_C, data.k * data.k * sizeof(int), cudaMemcpyHostToDevice);

        return data;
    }

    void writeOutput(const std::string &filename, const ConvolutionData &data)
    {
        // Copy result from device back to host
        cudaMemcpy(data.h_F, data.d_F, data.n * data.m * sizeof(int), cudaMemcpyDeviceToHost);

        std::ofstream fout(filename);
        if (!fout)
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            exit(1);
        }

        fout << data.n << " " << data.m << "\n";

        for (int i = 0; i < data.n; i++)
        {
            for (int j = 0; j < data.m; j++)
            {
                fout << data.h_F[i * data.m + j];
                if (j < data.m - 1)
                    fout << " ";
            }
            fout << "\n";
        }
        fout.close();
    }

    void freeData(ConvolutionData &data)
    {
        // free host memory

        delete[] data.h_F;
        delete[] data.h_C;

        cudaFree(data.d_F);
        cudaFree(data.d_C);
    }

    // CUDA KERNELS

    // Sequential in place: process one row at a time
    __global__ void convolutionRowKernel(int *F, int *C, int *lineBuffer,
                                         int row, int n, int m, int k)
    {
        int j = blockIdx.x * blockDim.x + threadIdx.x;

        if (j >= m)
            return; // boundary check

        int halfK = k / 2;
        int sum = 0;

        // apply convolution for element (row, j)
        for (int ki = 0; ki < k; ki++)
        {
            for (int kj = 0; kj < k; kj++)
            {
                int fi = row + ki - halfK;
                int fj = j + kj - halfK;

                // clamp indices
                int clampedFi = fi;
                int clampedFj = fj;
                if (clampedFi < 0)
                    clampedFi = 0;
                if (clampedFi >= n)
                    clampedFi = n - 1;
                if (clampedFj < 0)
                    clampedFj = 0;
                if (clampedFj >= m)
                    clampedFj = m - 1;

                // check if in buffered range
                int value;
                if (clampedFi >= row - k + 1 && clampedFi <= row)
                {
                    value = lineBuffer[(clampedFi % k) * m + clampedFj];
                }
                else
                {
                    value = F[clampedFi * m + clampedFj];
                }
                sum += value * C[ki * k + kj];
            }
        }
        // write result back to F
        F[row * m + j] = sum;
    }

    // parallel in-place: process entire matrix
    __global__ void convolutionParallelKernel(int *F_in, int *F_out, int *C,
                                              int n, int m, int k)
    {
        //each thread processes one element (i, j)
        int i = blockIdx.y * blockDim.y + threadIdx.y;
        int j = blockIdx.x * blockDim.x + threadIdx.x;

        if (i >= n || j >= m) return; //boundary check

        int halfK = k / 2;
        int sum = 0;

        //apply convolution for element (i, j)
        for(int ki = 0; ki < k; ki++){
            for(int kj = 0; kj < k; kj++){
                int fi = i + ki - halfK;
                int fj = j + kj - halfK;

                //clamp indices
                if (fi < 0) fi = 0;
                if (fi >= n) fi = n - 1;
                if (fj < 0) fj = 0;
                if (fj >= m) fj = m - 1;

                sum += F_in[fi * m + fj] * C[ki * k + kj];
            }
        }

        //write to output buffer
        F_out[i * m + j] = sum;
    }

    // ============================================================================
    // HOST FUNCTIONS THAT LAUNCH KERNELS
    // ============================================================================

    void applyConvolutionInPlace(ConvolutionData &data)
    {
        int n = data.n, m = data.m, k = data.k;

        // Allocate line buffer on device (k rows of m columns)
        int *d_lineBuffer;
        cudaMalloc(&d_lineBuffer, k * m * sizeof(int));

        // Initialize buffer with first k-1 rows
        int rowsToInit = std::min(k - 1, n);
        for (int i = 0; i < rowsToInit; i++)
        {
            cudaMemcpy(d_lineBuffer + i * m,
                       data.d_F + i * m,
                       m * sizeof(int),
                       cudaMemcpyDeviceToDevice);
        }

        // Configure kernel launch parameters
        int threadsPerBlock = 256;
        int numBlocks = (m + threadsPerBlock - 1) / threadsPerBlock;

        // Process each row sequentially
        for (int row = 0; row < n; row++)
        {
            // Update line buffer with current row (before it gets overwritten)
            cudaMemcpy(d_lineBuffer + (row % k) * m,
                       data.d_F + row * m,
                       m * sizeof(int),
                       cudaMemcpyDeviceToDevice);

            // Launch kernel to process this row
            convolutionRowKernel<<<numBlocks, threadsPerBlock>>>(
                data.d_F, data.d_C, d_lineBuffer, row, n, m, k);

            // Wait for this row to complete before moving to next
            cudaDeviceSynchronize();
        }

        // Free line buffer
        cudaFree(d_lineBuffer);
    }

    void applyConvolutionInPlaceParallel(ConvolutionData &data, int numBlocks)
    {
        int n = data.n, m = data.m, k = data.k;

        // Allocate output buffer on device (for double buffering)
        int *d_F_out;
        cudaError_t err = cudaMalloc(&d_F_out, n * m * sizeof(int));
        if (err != cudaSuccess) {
            std::cerr << "cudaMalloc failed: " << cudaGetErrorString(err) << std::endl;
            return;
        }

        // Configure 2D grid of thread blocks
        // 16x16 = 256 threads per block
        dim3 threadsPerBlock(16, 16);
        dim3 gridSize((m + 15) / 16, (n + 15) / 16);

        std::cout << "DEBUG: Parallel kernel launch - grid(" << gridSize.x << "," << gridSize.y
                  << ") threads(" << threadsPerBlock.x << "," << threadsPerBlock.y
                  << ") n=" << n << " m=" << m << " k=" << k << std::endl;

        // Launch parallel kernel (processes entire matrix at once)
        convolutionParallelKernel<<<gridSize, threadsPerBlock>>>(
            data.d_F,  // Input
            d_F_out,   // Output (separate buffer)
            data.d_C,  // Convolution kernel
            n, m, k);

        // Check for kernel launch errors
        err = cudaGetLastError();
        if (err != cudaSuccess) {
            std::cerr << "Kernel launch failed: " << cudaGetErrorString(err) << std::endl;
            cudaFree(d_F_out);
            return;
        }

        // Wait for kernel to complete
        err = cudaDeviceSynchronize();
        if (err != cudaSuccess) {
            std::cerr << "Kernel execution failed: " << cudaGetErrorString(err) << std::endl;
            cudaFree(d_F_out);
            return;
        }

        // Copy result back to d_F (making it "in-place" from outside perspective)
        cudaMemcpy(data.d_F, d_F_out, n * m * sizeof(int),
                   cudaMemcpyDeviceToDevice);

        // Free temporary buffer
        cudaFree(d_F_out);
    }

} // namespace CudaImpl
