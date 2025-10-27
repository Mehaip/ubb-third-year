#include "convolution.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

namespace VectorImpl {

static int getElement(const ConvolutionData& data, int i, int j) {
    if (i < 0) i = 0;
    if (i >= data.n) i = data.n - 1;
    if (j < 0) j = 0;
    if (j >= data.m) j = data.m - 1;
    return data.F[i][j];
}

ConvolutionData readInput(const std::string& filename) {
    ConvolutionData data;
    
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }
    
    fin >> data.n >> data.m >> data.k;
    
    data.F.assign(data.n, std::vector<int>(data.m));
    data.V.assign(data.n, std::vector<int>(data.m));
    data.C.assign(data.k, std::vector<int>(data.k));
    
    for (int i = 0; i < data.n; i++) {
        for (int j = 0; j < data.m; j++) {
            fin >> data.F[i][j];
        }
    }
    
    for (int i = 0; i < data.k; i++) {
        for (int j = 0; j < data.k; j++) {
            fin >> data.C[i][j];
        }
    }
    
    fin.close();
    return data;
}

void writeOutput(const std::string& filename, const ConvolutionData& data) {
    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }
    
    fout << data.n << " " << data.m << "\n";
    for (int i = 0; i < data.n; i++) {
        for (int j = 0; j < data.m; j++) {
            fout << data.V[i][j];
            if (j < data.m - 1) fout << " ";
        }
        fout << "\n";
    }
    
    fout.close();
}

void applyConvolution(ConvolutionData& data) {
    int halfK = data.k / 2;
    
    for (int i = 0; i < data.n; i++) {
        for (int j = 0; j < data.m; j++) {
            int sum = 0;
            for (int ki = 0; ki < data.k; ki++) {
                for (int kj = 0; kj < data.k; kj++) {
                    int fi = i + ki - halfK;
                    int fj = j + kj - halfK;
                    sum += getElement(data, fi, fj) * data.C[ki][kj];
                }
            }
            data.V[i][j] = sum;
        }
    }
}

static void workerRows(ConvolutionData& data, int startRow, int endRow) {
    int halfK = data.k / 2;
    
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < data.m; j++) {
            int sum = 0;
            for (int ki = 0; ki < data.k; ki++) {
                for (int kj = 0; kj < data.k; kj++) {
                    int fi = i + ki - halfK;
                    int fj = j + kj - halfK;
                    sum += getElement(data, fi, fj) * data.C[ki][kj];
                }
            }
            data.V[i][j] = sum;
        }
    }
}

void applyConvolutionParallelHorizontal(ConvolutionData& data, int numThreads) {
    std::vector<std::thread> threads;
    
    int rowsPerThread = data.n / numThreads;
    int extraRows = data.n % numThreads;
    
    int startRow = 0;
    for (int t = 0; t < numThreads; t++) {
        int endRow = startRow + rowsPerThread + (t < extraRows ? 1 : 0);
        threads.emplace_back(workerRows, std::ref(data), startRow, endRow);
        startRow = endRow;
    }
    
    for (auto& th : threads) {
        th.join();
    }
}

static void workerColumns(ConvolutionData& data, int startCol, int endCol) {
    int halfK = data.k / 2;
    
    for (int i = 0; i < data.n; i++) {
        for (int j = startCol; j < endCol; j++) {
            int sum = 0;
            for (int ki = 0; ki < data.k; ki++) {
                for (int kj = 0; kj < data.k; kj++) {
                    int fi = i + ki - halfK;
                    int fj = j + kj - halfK;
                    sum += getElement(data, fi, fj) * data.C[ki][kj];
                }
            }
            data.V[i][j] = sum;
        }
    }
}

void applyConvolutionParallelVertical(ConvolutionData& data, int numThreads) {
    std::vector<std::thread> threads;
    
    int colsPerThread = data.m / numThreads;
    int extraCols = data.m % numThreads;
    
    int startCol = 0;
    for (int t = 0; t < numThreads; t++) {
        int endCol = startCol + colsPerThread + (t < extraCols ? 1 : 0);
        threads.emplace_back(workerColumns, std::ref(data), startCol, endCol);
        startCol = endCol;
    }
    
    for (auto& th : threads) {
        th.join();
    }
}

static void workerBlock(ConvolutionData& data, int startRow, int endRow, int startCol, int endCol) {
    int halfK = data.k / 2;
    
    for (int i = startRow; i < endRow; i++) {
        for (int j = startCol; j < endCol; j++) {
            int sum = 0;
            for (int ki = 0; ki < data.k; ki++) {
                for (int kj = 0; kj < data.k; kj++) {
                    int fi = i + ki - halfK;
                    int fj = j + kj - halfK;
                    sum += getElement(data, fi, fj) * data.C[ki][kj];
                }
            }
            data.V[i][j] = sum;
        }
    }
}


// Lab 2: In-place convolution (sequential)
void applyConvolutionInPlace(ConvolutionData& data) {
    int halfK = data.k / 2;
    ///sliding window
    std::vector<std::vector<int>> rowBuffers(data.k, std::vector<int>(data.m));

    // initialize buffers with first k rows
    for (int bufIdx = 0; bufIdx < data.k; bufIdx++) {
        int rowIdx = bufIdx - halfK;  // For k=3, this is -1, 0, 1
        if (rowIdx < 0) rowIdx = 0;
        if (rowIdx >= data.n) rowIdx = data.n - 1;

        rowBuffers[bufIdx] = data.F[rowIdx];
    }

    // process each row
    for (int i = 0; i < data.n; i++) {
        std::vector<int> resultRow(data.m);

        // calc conv pt acest rand
        for (int j = 0; j < data.m; j++) {
            int sum = 0;

            for (int ki = 0; ki < data.k; ki++) {
                for (int kj = 0; kj < data.k; kj++) {
                    int fj = j + kj - halfK;

                    // Clamp column index
                    if (fj < 0) fj = 0;
                    if (fj >= data.m) fj = data.m - 1;

                    sum += rowBuffers[ki][fj] * data.C[ki][kj];
                }
            }

            resultRow[j] = sum;
        }

        // scriem resultatele back to F[i]
        data.F[i] = resultRow;

        // Shift buffers 
        if (i < data.n - 1) {
            // Rotate buffers (move everything up one position)
            std::vector<int> temp = rowBuffers[0];
            for (int bufIdx = 0; bufIdx < data.k - 1; bufIdx++) {
                rowBuffers[bufIdx] = rowBuffers[bufIdx + 1];
            }
            rowBuffers[data.k - 1] = temp;

            // Load next row
            int nextRowIdx = i + halfK + 1;
            if (nextRowIdx >= data.n) nextRowIdx = data.n - 1;

            rowBuffers[data.k - 1] = data.F[nextRowIdx];
        }
    }

    // Copy F to V for consistency with interface
    data.V = data.F;
}

// Lab 2: In-place convolution (parallel horizontal)
static void workerRowsInPlace(ConvolutionData& data, int startRow, int endRow,
                              const std::vector<std::vector<int>>& originalF) {
    int halfK = data.k / 2;

    for (int i = startRow; i < endRow; i++) {
        std::vector<int> resultRow(data.m);

        for (int j = 0; j < data.m; j++) {
            int sum = 0;

            for (int ki = 0; ki < data.k; ki++) {
                for (int kj = 0; kj < data.k; kj++) {
                    int fi = i + ki - halfK;
                    int fj = j + kj - halfK;

                   
                    if (fi < 0) fi = 0;
                    if (fi >= data.n) fi = data.n - 1;
                    if (fj < 0) fj = 0;
                    if (fj >= data.m) fj = data.m - 1;

                    sum += originalF[fi][fj] * data.C[ki][kj];
                }
            }

            resultRow[j] = sum;
        }

        
        data.F[i] = resultRow;
    }
}

void applyConvolutionInPlaceParallelHorizontal(ConvolutionData& data, int numThreads) {
    std::vector<std::vector<int>> originalF = data.F;

    std::vector<std::thread> threads;

    int rowsPerThread = data.n / numThreads;
    int extraRows = data.n % numThreads;

    int startRow = 0;
    for (int t = 0; t < numThreads; t++) {
        int endRow = startRow + rowsPerThread + (t < extraRows ? 1 : 0);
        threads.emplace_back(workerRowsInPlace, std::ref(data), startRow, endRow, std::cref(originalF));
        startRow = endRow;
    }

    for (auto& th : threads) {
        th.join();
    }

    data.V = data.F;
}

} // namespace VectorImpl