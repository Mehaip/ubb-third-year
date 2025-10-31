#include "convolution.h"
#include "barrier.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

namespace VectorImpl {

// Custom barrier implementation for thread synchronization
class Barrier {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
    int waiting;
    int generation;

public:
    explicit Barrier(int count) : count(count), waiting(0), generation(0) {}

    void arrive_and_wait() {
        std::unique_lock<std::mutex> lock(mtx);
        int gen = generation;

        if (++waiting == count) {
            generation++;
            waiting = 0;
            cv.notify_all();
        } else {
            cv.wait(lock, [this, gen] { return gen != generation; });
        }
    }
};

static int getElement(const ConvolutionData& data, int i, int j) {
    if (i < 0) i = 0;
    if (i >= data.n) i = data.n - 1;
    if (j < 0) j = 0;
    if (j >= data.m) j = data.m - 1;
    return data.F[i][j];
}

// Helper function for in-place convolution that uses lineBuffer when appropriate
static int getElementInPlace(const ConvolutionData& data, int fi, int fj,
                              const std::vector<std::vector<int>>& lineBuffer,
                              int currentRow, int k, int m) {
    // Clamp indices
    int clampedFi = fi;
    int clampedFj = fj;
    if (clampedFi < 0) clampedFi = 0;
    if (clampedFi >= data.n) clampedFi = data.n - 1;
    if (clampedFj < 0) clampedFj = 0;
    if (clampedFj >= m) clampedFj = m - 1;

    // Check if clamped index is in the buffered range
    if (clampedFi >= currentRow - k + 1 && clampedFi <= currentRow) {
        return lineBuffer[clampedFi % k][clampedFj];
    } else {
        // Not in buffered range, read from F (should be unprocessed or safe)
        return data.F[clampedFi][clampedFj];
    }
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

static void workerInPlaceBarrier(ConvolutionData& data, int startRow, int endRow,
                                  Barrier& barrier, std::vector<int>& topBorder,
                                  std::vector<int>& bottomBorder) {
    int halfK = data.k / 2;
    int m = data.m, k = data.k;

    std::vector<int> newLine(m);

    int borderRows = std::min(halfK, endRow - startRow);
    for (int i = startRow; i < startRow + borderRows; i++) {
        for (int j = 0; j < m; j++) {
            int sum = 0;
            for (int ki = 0; ki < k; ki++) {
                for (int kj = 0; kj < k; kj++) {
                    int fi = i + ki - halfK;
                    int fj = j + kj - halfK;
                    sum += getElement(data, fi, fj) * data.C[ki][kj];
                }
            }
            topBorder[(i - startRow) * m + j] = sum;
        }
    }

    int bottomStart = std::max(startRow, endRow - halfK);
    for (int i = bottomStart; i < endRow; i++) {
        for (int j = 0; j < m; j++) {
            int sum = 0;
            for (int ki = 0; ki < k; ki++) {
                for (int kj = 0; kj < k; kj++) {
                    int fi = i + ki - halfK;
                    int fj = j + kj - halfK;
                    sum += getElement(data, fi, fj) * data.C[ki][kj];
                }
            }
            bottomBorder[(i - bottomStart) * m + j] = sum;
        }
    }

    barrier.arrive_and_wait();

    std::vector<std::vector<int>> lineBuffer(k, std::vector<int>(m));

    for (int i = startRow; i < endRow; i++) {
        bool isTopBorder = (i < startRow + halfK);
        bool isBottomBorder = (i >= endRow - halfK);

        if (isTopBorder) {
            for (int j = 0; j < m; j++) {
                data.F[i][j] = topBorder[(i - startRow) * m + j];
            }
        } else if (isBottomBorder) {
            for (int j = 0; j < m; j++) {
                data.F[i][j] = bottomBorder[(i - (endRow - halfK)) * m + j];
            }
        } else {
            lineBuffer[i % k] = data.F[i];

            for (int j = 0; j < m; j++) {
                int sum = 0;
                for (int ki = 0; ki < k; ki++) {
                    for (int kj = 0; kj < k; kj++) {
                        int fi = i + ki - halfK;
                        int fj = j + kj - halfK;

                        int value = getElementInPlace(data, fi, fj, lineBuffer, i, k, m);
                        sum += value * data.C[ki][kj];
                    }
                }
                newLine[j] = sum;
            }

            data.F[i] = newLine;
        }
    }
}

void applyConvolutionInPlaceParallelBarrier(ConvolutionData& data, int numThreads) {
    std::vector<std::thread> threads;
    Barrier barrier(numThreads);

    int rowsPerThread = data.n / numThreads;
    int extraRows = data.n % numThreads;
    int halfK = data.k / 2;

    std::vector<std::vector<int>> topBorders(numThreads);
    std::vector<std::vector<int>> bottomBorders(numThreads);

    int startRow = 0;
    for (int t = 0; t < numThreads; t++) {
        int endRow = startRow + rowsPerThread + (t < extraRows ? 1 : 0);

        topBorders[t].resize(halfK * data.m);
        bottomBorders[t].resize(halfK * data.m);

        threads.emplace_back(workerInPlaceBarrier, std::ref(data), startRow, endRow,
                            std::ref(barrier), std::ref(topBorders[t]),
                            std::ref(bottomBorders[t]));
        startRow = endRow;
    }

    for (auto& th : threads) {
        th.join();
    }

    // Copy results from F to V for output
    data.V = data.F;
}

void applyConvolutionInPlace(ConvolutionData& data) {
    int halfK = data.k / 2;
    int n = data.n, m = data.m, k = data.k;

    std::vector<std::vector<int>> lineBuffer(k, std::vector<int>(m));
    for (int i = 0; i < std::min(k - 1, n); i++) {
        lineBuffer[i] = data.F[i];
    }

    std::vector<int> newLine(m);

    for (int i = 0; i < n; i++) {
        lineBuffer[i % k] = data.F[i];

        for (int j = 0; j < m; j++) {
            int sum = 0;

            for (int ki = 0; ki < k; ki++) {
                for (int kj = 0; kj < k; kj++) {
                    int fi = i + ki - halfK;
                    int fj = j + kj - halfK;

                    int value = getElementInPlace(data, fi, fj, lineBuffer, i, k, m);
                    sum += value * data.C[ki][kj];
                }
            }

            newLine[j] = sum;
        }

        data.F[i] = newLine;
    }

    // Copy results from F to V for output
    data.V = data.F;
}

static void workerInPlace(ConvolutionData& data, int startRow, int endRow) {
    int halfK = data.k / 2;
    int m = data.m, k = data.k;

    std::vector<std::vector<int>> lineBuffer(k, std::vector<int>(m));
    std::vector<int> newLine(m);

    for (int i = startRow; i < endRow; i++) {
        lineBuffer[i % k] = data.F[i];

        for (int j = 0; j < m; j++) {
            int sum = 0;

            for (int ki = 0; ki < k; ki++) {
                for (int kj = 0; kj < k; kj++) {
                    int fi = i + ki - halfK;
                    int fj = j + kj - halfK;

                    int value = getElementInPlace(data, fi, fj, lineBuffer, i, k, m);
                    sum += value * data.C[ki][kj];
                }
            }

            newLine[j] = sum;
        }

        data.F[i] = newLine;
    }
}

void applyConvolutionInPlaceParallel(ConvolutionData& data, int numThreads) {
    std::vector<std::thread> threads;

    int rowsPerThread = data.n / numThreads;
    int extraRows = data.n % numThreads;

    int startRow = 0;
    for (int t = 0; t < numThreads; t++) {
        int endRow = startRow + rowsPerThread + (t < extraRows ? 1 : 0);
        threads.emplace_back(workerInPlace, std::ref(data), startRow, endRow);
        startRow = endRow;
    }

    for (auto& th : threads) {
        th.join();
    }

    // Copy results from F to V for output
    data.V = data.F;
}

} // namespace VectorImpl