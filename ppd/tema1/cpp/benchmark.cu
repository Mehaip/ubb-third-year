#include "convolution.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace std::chrono;
using namespace CudaImpl;

void printGpuInfo()
{
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);

    if (deviceCount == 0)
    {
        cerr << "No CUDA devices found!" << endl;
        exit(1);
    }

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    cout << "GPU: " << prop.name << endl;
    cout << "Compute Capability: " << prop.major << "." << prop.minor << endl;
    cout << "Total Global Memory: " << prop.totalGlobalMem / (1024 * 1024) << " MB" << endl;
    cout << "Max Threads Per Block: " << prop.maxThreadsPerBlock << endl;
}

void runTest(int N, int M, int k, int blockCounts[], int numBlockCounts,
             const string &inputFile, const string &outputFile, const string &outputFileParallel)
{
    cout << "\n" << string(60, '=') << "\n";
    cout << "Testing with N=" << N << ", M=" << M << ", k=" << k << "\n";
    cout << string(60, '=') << "\n";

    const int NUM_RUNS = 10;

    // Sequential in-place - run NUM_RUNS times and average
    long long sequentialTotal = 0;

    for (int run = 0; run < NUM_RUNS; run++)
    {
        ConvolutionData data = readInput(inputFile);
        auto start = high_resolution_clock::now();
        applyConvolutionInPlace(data);
        auto end = high_resolution_clock::now();
        sequentialTotal += duration_cast<milliseconds>(end - start).count();
        freeData(data);
    }

    double sequentialAvg = (double)sequentialTotal / NUM_RUNS;
    cout << "Sequential in-place (avg of " << NUM_RUNS << " runs): "
         << fixed << setprecision(2) << sequentialAvg << " ms\n";

    // Write output file once (for verification)
    ConvolutionData dataForOutput = readInput(inputFile);
    applyConvolutionInPlace(dataForOutput);
    writeOutput(outputFile, dataForOutput);
    freeData(dataForOutput);

    // Test with different block counts
    for (int i = 0; i < numBlockCounts; i++)
    {
        int p = blockCounts[i];
        cout << "\n  [blocks=" << p << "]\n";

        // Parallel - run NUM_RUNS times and average
        long long parallelTotal = 0;
        for (int run = 0; run < NUM_RUNS; run++)
        {
            ConvolutionData data = readInput(inputFile);
            auto start = high_resolution_clock::now();
            applyConvolutionInPlaceParallel(data, p);
            auto end = high_resolution_clock::now();
            parallelTotal += duration_cast<milliseconds>(end - start).count();
            freeData(data);
        }
        double parallelAvg = (double)parallelTotal / NUM_RUNS;
        double speedup = sequentialAvg / parallelAvg;
        cout << "    Parallel in-place (avg of " << NUM_RUNS << " runs): "
             << fixed << setprecision(2) << parallelAvg << " ms (speedup: "
             << fixed << setprecision(2) << speedup << "x)\n";
    }

    // Write parallel output file once (for verification)
    ConvolutionData dataForOutputParallel = readInput(inputFile);
    applyConvolutionInPlaceParallel(dataForOutputParallel, 2);
    writeOutput(outputFileParallel, dataForOutputParallel);
    freeData(dataForOutputParallel);
}

int main()
{
    cout << string(60, '=') << "\n";
    cout << "PPD - Matrix Convolution Benchmark - CUDA\n";
    cout << string(60, '=') << "\n";

    // Print GPU info
    printGpuInfo();

    // Test 1: N=M=10, k=3
    int blocks1[] = {2};
    runTest(10, 10, 3, blocks1, 1,
            "data/matrice10.txt",
            "results/cpp/sequential/output10.txt",
            "results/cpp/parallel/output10.txt");

    // Test 2: N=M=1000, k=3
    int blocks2[] = {2, 4, 8, 16};
    runTest(1000, 1000, 3, blocks2, 4,
            "data/matrice1000.txt",
            "results/cpp/sequential/output1000.txt",
            "results/cpp/parallel/output1000.txt");

    // Test 3: N=M=10000, k=3
    int blocks3[] = {2, 4, 8, 16};
    runTest(10000, 10000, 3, blocks3, 4,
            "data/matrice10000.txt",
            "results/cpp/sequential/output10000.txt",
            "results/cpp/parallel/output10000.txt");

    cout << "\n" << string(60, '=') << "\n";
    cout << "All tests completed!\n";
    cout << string(60, '=') << "\n";

    return 0;
}
