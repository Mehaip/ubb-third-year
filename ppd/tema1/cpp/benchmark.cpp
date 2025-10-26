#include "convolution.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <random>
#include <fstream>

using namespace std;
using namespace std::chrono;

void generateTestData(const string& filename, int n, int m, int k) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Failed to create test data file: " << filename << "\n";
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 10);

    // Write dimensions
    out << n << " " << m << " " << k << "\n";

    // Write matrix F (n x m)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            out << dis(gen);
            if (j < m - 1) out << " ";
        }
        out << "\n";
    }

    // Write kernel C (k x k)
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            out << dis(gen);
            if (j < k - 1) out << " ";
        }
        out << "\n";
    }

    out.close();
}

void runTest(int N, int M, int k, int threadCounts[], int numThreadCounts) {
    cout << "\n" << string(60, '=') << "\n";
    cout << "Testing with N=" << N << ", M=" << M << ", k=" << k << "\n";
    cout << string(60, '=') << "\n";

    const int NUM_RUNS = 10;
    string inputFile = "../data/date.txt";
    string outputFile = "../data/output.txt";

    try {
        // Generate and write input file
        generateTestData(inputFile, N, M, k);

        // Sequential (no threads) - run 10 times and average
        long long sequentialTotal = 0;

        for (int run = 0; run < NUM_RUNS; run++) {
            VectorImpl::ConvolutionData data = VectorImpl::readInput(inputFile);
            auto start = high_resolution_clock::now();
            VectorImpl::applyConvolution(data);
            auto end = high_resolution_clock::now();
            sequentialTotal += duration_cast<milliseconds>(end - start).count();
        }

        double sequentialAvg = (double)sequentialTotal / NUM_RUNS;
        cout << "Sequential (avg of " << NUM_RUNS << " runs): "
             << fixed << setprecision(2) << sequentialAvg << " ms\n";

        // Write output file once (for verification)
        VectorImpl::ConvolutionData dataForOutput = VectorImpl::readInput(inputFile);
        VectorImpl::applyConvolution(dataForOutput);
        VectorImpl::writeOutput(outputFile, dataForOutput);

        // Test with different thread counts
        for (int i = 0; i < numThreadCounts; i++) {
            int p = threadCounts[i];
            cout << "\n  [p=" << p << " threads]\n";

            // Horizontal strategy - run 10 times and average
            long long horizontalTotal = 0;
            for (int run = 0; run < NUM_RUNS; run++) {
                VectorImpl::ConvolutionData data = VectorImpl::readInput(inputFile);
                auto start = high_resolution_clock::now();
                VectorImpl::applyConvolutionParallelHorizontal(data, p);
                auto end = high_resolution_clock::now();
                horizontalTotal += duration_cast<milliseconds>(end - start).count();
            }
            double horizontalAvg = (double)horizontalTotal / NUM_RUNS;
            double horizontalSpeedup = sequentialAvg / horizontalAvg;
            cout << "    Horizontal (avg of " << NUM_RUNS << " runs): "
                 << fixed << setprecision(2) << horizontalAvg << " ms (speedup: "
                 << fixed << setprecision(2) << horizontalSpeedup << "x)\n";

            // Vertical strategy - run 10 times and average
            long long verticalTotal = 0;
            for (int run = 0; run < NUM_RUNS; run++) {
                VectorImpl::ConvolutionData data = VectorImpl::readInput(inputFile);
                auto start = high_resolution_clock::now();
                VectorImpl::applyConvolutionParallelVertical(data, p);
                auto end = high_resolution_clock::now();
                verticalTotal += duration_cast<milliseconds>(end - start).count();
            }
            double verticalAvg = (double)verticalTotal / NUM_RUNS;
            double verticalSpeedup = sequentialAvg / verticalAvg;
            cout << "    Vertical (avg of " << NUM_RUNS << " runs): "
                 << fixed << setprecision(2) << verticalAvg << " ms (speedup: "
                 << fixed << setprecision(2) << verticalSpeedup << "x)\n";
        }

    } catch (const exception& e) {
        cout << "Error during test: " << e.what() << "\n";
    }
}

int main() {
    cout << "PPD - Tema1 - Comprehensive Testing\n";

    // Test 1: N=M=10, k=3, p=4
    int threads1[] = {4};
    runTest(10, 10, 3, threads1, 1);

    // Test 2: N=M=1000, k=5, p=2,4,8,16
    int threads2[] = {2, 4, 8, 16};
    runTest(1000, 1000, 5, threads2, 4);

    // Test 3: N=10, M=10000, k=5, p=2,4,8,16
    runTest(10, 10000, 5, threads2, 4);

    // Test 4: N=10000, M=10, k=5, p=2,4,8,16
    runTest(10000, 10, 5, threads2, 4);

    // Test 5: N=10000, M=10000, k=5, p=2,4,8,16
    runTest(10000, 10000, 5, threads2, 4);

    cout << "\n" << string(60, '=') << "\n";
    cout << "All tests completed!\n";
    cout << string(60, '=') << "\n";

    return 0;
}
