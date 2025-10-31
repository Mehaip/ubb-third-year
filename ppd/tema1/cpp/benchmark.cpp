#include "convolution.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <random>
#include <fstream>

using namespace std;
using namespace std::chrono;


void runTestLab2(int N, int M, int k, int threadCounts[], int numThreadCounts, const string& inputFile, const string& outputFile) {
    cout << "\n" << string(60, '=') << "\n";
    cout << "Testing with N=" << N << ", M=" << M << ", k=" << k << "\n";
    cout << string(60, '=') << "\n";

    const int NUM_RUNS = 1;

    try {
       

        // Sequential in-place - run 10 times and average
        long long sequentialTotal = 0;

        for (int run = 0; run < NUM_RUNS; run++) {
            VectorImpl::ConvolutionData data = VectorImpl::readInput(inputFile);
            auto start = high_resolution_clock::now();
            VectorImpl::applyConvolutionInPlace(data);
            auto end = high_resolution_clock::now();
            sequentialTotal += duration_cast<milliseconds>(end - start).count();
        }

        double sequentialAvg = (double)sequentialTotal / NUM_RUNS;
        cout << "Sequential in-place (avg of " << NUM_RUNS << " runs): "
             << fixed << setprecision(2) << sequentialAvg << " ms\n";

        // Write output file once (for verification)
        VectorImpl::ConvolutionData dataForOutput = VectorImpl::readInput(inputFile);
        VectorImpl::applyConvolutionInPlace(dataForOutput);
        VectorImpl::writeOutput(outputFile, dataForOutput);

        // Test with different thread counts (horizontal only for Lab 2)
        for (int i = 0; i < numThreadCounts; i++) {
            int p = threadCounts[i];
            cout << "\n  [p=" << p << " threads]\n";

            // Horizontal strategy - run 10 times and average
            long long horizontalTotal = 0;
            for (int run = 0; run < NUM_RUNS; run++) {
                VectorImpl::ConvolutionData data = VectorImpl::readInput(inputFile);
                auto start = high_resolution_clock::now();
                VectorImpl::applyConvolutionInPlaceParallel(data, p);
                auto end = high_resolution_clock::now();
                horizontalTotal += duration_cast<milliseconds>(end - start).count();
            }
            double horizontalAvg = (double)horizontalTotal / NUM_RUNS;
            double horizontalSpeedup = sequentialAvg / horizontalAvg;
            cout << "    Horizontal in-place (avg of " << NUM_RUNS << " runs): "
                 << fixed << setprecision(2) << horizontalAvg << " ms (speedup: "
                 << fixed << setprecision(2) << horizontalSpeedup << "x)\n";
        }

    } catch (const exception& e) {
        cout << "Error during test: " << e.what() << "\n";
    }
}

void runTestLab1(int N, int M, int k, int threadCounts[], int numThreadCounts) {
    cout << "\n" << string(60, '=') << "\n";
    cout << "Testing with N=" << N << ", M=" << M << ", k=" << k << "\n";
    cout << string(60, '=') << "\n";

    const int NUM_RUNS = 1;
    string inputFile = "../data/date.txt";
    string outputFile = "../data/outputcpp.txt";

    try {
        // Generate and write input file
        //generateTestData(inputFile, N, M, k);

        // ============================================================
        // VECTOR IMPLEMENTATION
        // ============================================================
        cout << "\n[Vector Implementation]\n";

        // Sequential (no threads) - run 10 times and average
        long long vectorSequentialTotal = 0;

        for (int run = 0; run < NUM_RUNS; run++) {
            VectorImpl::ConvolutionData data = VectorImpl::readInput(inputFile);
            auto start = high_resolution_clock::now();
            VectorImpl::applyConvolution(data);
            auto end = high_resolution_clock::now();
            vectorSequentialTotal += duration_cast<milliseconds>(end - start).count();
        }

        double vectorSequentialAvg = (double)vectorSequentialTotal / NUM_RUNS;
        cout << "Sequential (avg of " << NUM_RUNS << " runs): "
             << fixed << setprecision(2) << vectorSequentialAvg << " ms\n";

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
            double horizontalSpeedup = vectorSequentialAvg / horizontalAvg;
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
            double verticalSpeedup = vectorSequentialAvg / verticalAvg;
            cout << "    Vertical (avg of " << NUM_RUNS << " runs): "
                 << fixed << setprecision(2) << verticalAvg << " ms (speedup: "
                 << fixed << setprecision(2) << verticalSpeedup << "x)\n";
        }

        // ============================================================
        // STATIC ARRAY IMPLEMENTATION
        // ============================================================
        cout << "\n[Static Array Implementation]\n";

        // Sequential (no threads) - run 10 times and average
        long long staticSequentialTotal = 0;

        for (int run = 0; run < NUM_RUNS; run++) {
            int n, m, kVal;
            StaticArray::readInput(inputFile, n, m, kVal);
            auto start = high_resolution_clock::now();
            StaticArray::applyConvolution(n, m, kVal);
            auto end = high_resolution_clock::now();
            staticSequentialTotal += duration_cast<milliseconds>(end - start).count();
        }

        double staticSequentialAvg = (double)staticSequentialTotal / NUM_RUNS;
        cout << "Sequential (avg of " << NUM_RUNS << " runs): "
             << fixed << setprecision(2) << staticSequentialAvg << " ms\n";

        // Test with different thread counts
        for (int i = 0; i < numThreadCounts; i++) {
            int p = threadCounts[i];
            cout << "\n  [p=" << p << " threads]\n";

            // Horizontal strategy - run 10 times and average
            long long horizontalTotal = 0;
            for (int run = 0; run < NUM_RUNS; run++) {
                int n, m, kVal;
                StaticArray::readInput(inputFile, n, m, kVal);
                auto start = high_resolution_clock::now();
                StaticArray::applyConvolutionParallelHorizontal(n, m, kVal, p);
                auto end = high_resolution_clock::now();
                horizontalTotal += duration_cast<milliseconds>(end - start).count();
            }
            double horizontalAvg = (double)horizontalTotal / NUM_RUNS;
            double horizontalSpeedup = staticSequentialAvg / horizontalAvg;
            cout << "    Horizontal (avg of " << NUM_RUNS << " runs): "
                 << fixed << setprecision(2) << horizontalAvg << " ms (speedup: "
                 << fixed << setprecision(2) << horizontalSpeedup << "x)\n";

            // Vertical strategy - run 10 times and average
            long long verticalTotal = 0;
            for (int run = 0; run < NUM_RUNS; run++) {
                int n, m, kVal;
                StaticArray::readInput(inputFile, n, m, kVal);
                auto start = high_resolution_clock::now();
                StaticArray::applyConvolutionParallelVertical(n, m, kVal, p);
                auto end = high_resolution_clock::now();
                verticalTotal += duration_cast<milliseconds>(end - start).count();
            }
            double verticalAvg = (double)verticalTotal / NUM_RUNS;
            double verticalSpeedup = staticSequentialAvg / verticalAvg;
            cout << "    Vertical (avg of " << NUM_RUNS << " runs): "
                 << fixed << setprecision(2) << verticalAvg << " ms (speedup: "
                 << fixed << setprecision(2) << verticalSpeedup << "x)\n";
        }

    } catch (const exception& e) {
        cout << "Error during test: " << e.what() << "\n";
    }
}

int main() {
    cout << string(60, '=') << "\n";
    cout << "PPD - Matrix Convolution Benchmark - Lab 2\n";
    cout << string(60, '=') << "\n";

    // Test 1: N=M=10, k=3, p=2
    int threads1[] = {2};
    runTestLab2(10, 10, 3, threads1, 1, "data/matrice10.txt", "results/cpp/output10.txt");

    // Test 2: N=M=1000, k=3, p=2,4,8,16
    int threads2[] = {2, 4, 8, 16};
    runTestLab2(1000, 1000, 3, threads2, 4, "data/matrice1000.txt", "results/cpp/output1000.txt");

    // Test 3: N=M=10000, k=3, p=2,4,8,16
    int threads3[] = {2, 4, 8, 16};
    runTestLab2(10000, 10000, 3, threads3, 4, "data/matrice10000.txt", "results/cpp/output10000.txt");

    cout << "\n" << string(60, '=') << "\n";
    cout << "All tests completed!\n";
    cout << string(60, '=') << "\n";

    return 0;
}
