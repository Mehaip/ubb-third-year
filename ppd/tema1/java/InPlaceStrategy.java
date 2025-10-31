import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.BrokenBarrierException;

public class InPlaceStrategy implements ConvolutionStrategy {
    private final int numThreads;

    public InPlaceStrategy(int numThreads) {
        this.numThreads = numThreads;
    }

    private int getElement(int[][] F, int i, int j, int n, int m) {
        if (i < 0) i = 0;
        if (i >= n) i = n - 1;
        if (j < 0) j = 0;
        if (j >= m) j = m - 1;
        return F[i][j];
    }

    // Helper function for in-place convolution that uses lineBuffer when appropriate
    private int getElementInPlace(int[][] F, int fi, int fj, int[][] lineBuffer,
                                   int currentRow, int k, int n, int m) {
        // Clamp indices
        int clampedFi = fi;
        int clampedFj = fj;
        if (clampedFi < 0) clampedFi = 0;
        if (clampedFi >= n) clampedFi = n - 1;
        if (clampedFj < 0) clampedFj = 0;
        if (clampedFj >= m) clampedFj = m - 1;

        // Check if clamped index is in the buffered range
        if (clampedFi >= currentRow - k + 1 && clampedFi <= currentRow) {
            return lineBuffer[clampedFi % k][clampedFj];
        } else {
            // Not in buffered range, read from F (should be unprocessed or safe)
            return F[clampedFi][clampedFj];
        }
    }

    @Override
    public void applyConvolution(int[][] F, int[][] V, int[][] C, int n, int m, int k) {
        k = 3;  // Always use k = 3
        int halfK = k / 2;

        if (numThreads <= 1) {
            applySequential(F, C, n, m, k, halfK);
        } else {
            applyParallelWithBarrier(F, C, n, m, k, halfK);
        }
    }

    private void applySequential(int[][] F, int[][] C, int n, int m, int k, int halfK) {
        int[][] lineBuffer = new int[k][m];

        for (int i = 0; i < Math.min(k - 1, n); i++) {
            System.arraycopy(F[i], 0, lineBuffer[i], 0, m);
        }

        int[] newLine = new int[m];

        for (int i = 0; i < n; i++) {
            System.arraycopy(F[i], 0, lineBuffer[i % k], 0, m);

            for (int j = 0; j < m; j++) {
                int sum = 0;

                for (int ki = 0; ki < k; ki++) {
                    for (int kj = 0; kj < k; kj++) {
                        int fi = i + ki - halfK;
                        int fj = j + kj - halfK;

                        int value = getElementInPlace(F, fi, fj, lineBuffer, i, k, n, m);
                        sum += value * C[ki][kj];
                    }
                }

                newLine[j] = sum;
            }

            System.arraycopy(newLine, 0, F[i], 0, m);
        }
    }

    private void applyParallelWithBarrier(int[][] F, int[][] C, int n, int m, int k, int halfK) {
        CyclicBarrier barrier = new CyclicBarrier(numThreads);
        Thread[] threads = new Thread[numThreads];
        //10x10
        int rowsPerThread = n / numThreads;
        int extraRows = n % numThreads;

        int[][] topBorders = new int[numThreads][m];//int[4][10]
        int[][] bottomBorders = new int[numThreads][m];//int[4][10]

        int startRow = 0;
        for (int t = 0; t < numThreads; t++) {
            int endRow = startRow + rowsPerThread + (t < extraRows ? 1 : 0);

            final int threadId = t; //no of thread
            final int threadStart = startRow; //start thread
            final int threadEnd = endRow; ///end of the thread

            threads[t] = new Thread(() -> {

                System.arraycopy(F[threadStart], 0, topBorders[threadId], 0, m); ///salvam topROw
            
                System.arraycopy(F[threadEnd-1], 0, bottomBorders[threadId], 0, m);
            

                try {
                    barrier.await(); ///thread waits here until numThreads call await()
                } catch (InterruptedException | BrokenBarrierException e) {
                    e.printStackTrace();
                }

                int[][] lineBuffer = new int[k][m];
                int[] newLine = new int[m];

                // Process all rows uniformly (including borders)
                for (int i = threadStart; i < threadEnd; i++) {
                    // Setup lineBuffer with the 3 rows needed for convolution
                    if (i == threadStart) {
                        // First row of this thread
                        if (threadId == 0) {
                            // First thread: clamp top (use row 0 twice)
                            System.arraycopy(topBorders[threadId], 0, lineBuffer[0], 0, m);
                            System.arraycopy(F[i], 0, lineBuffer[1], 0, m);
                            if (i + 1 < n) {
                                System.arraycopy(F[i + 1], 0, lineBuffer[2], 0, m);
                            } else {
                                System.arraycopy(F[i], 0, lineBuffer[2], 0, m);
                            }
                        } else {
                            // Not first thread: use previous thread's bottom border
                            System.arraycopy(bottomBorders[threadId - 1], 0, lineBuffer[0], 0, m);
                            System.arraycopy(F[i], 0, lineBuffer[1], 0, m);
                            if (i + 1 < threadEnd) {
                                System.arraycopy(F[i + 1], 0, lineBuffer[2], 0, m);
                            } else if (threadId < numThreads - 1) {
                                System.arraycopy(topBorders[threadId + 1], 0, lineBuffer[2], 0, m);
                            } else {
                                System.arraycopy(F[i], 0, lineBuffer[2], 0, m);
                            }
                        }
                    } else {
                        // Middle or last rows: rotate the buffer
                        System.arraycopy(lineBuffer[1], 0, lineBuffer[0], 0, m);
                        System.arraycopy(lineBuffer[2], 0, lineBuffer[1], 0, m);

                        // Get next row
                        if (i == threadEnd - 1) {
                            // Last row of this thread
                            if (threadId == numThreads - 1) {
                                // Last thread: clamp bottom
                                System.arraycopy(bottomBorders[threadId], 0, lineBuffer[2], 0, m);
                            } else {
                                // Use next thread's top border
                                System.arraycopy(topBorders[threadId + 1], 0, lineBuffer[2], 0, m);
                            }
                        } else {
                            // Normal case: next row is within range
                            System.arraycopy(F[i + 1], 0, lineBuffer[2], 0, m);
                        }
                    }

                    // Now compute convolution for row i using lineBuffer
                    for (int j = 0; j < m; j++) {
                        int sum = 0;

                        for (int ki = 0; ki < k; ki++) {
                            for (int kj = 0; kj < k; kj++) {
                                int fj = j + kj - halfK;

                                // Clamp column index
                                int clampedFj = fj;
                                if (clampedFj < 0) clampedFj = 0;
                                if (clampedFj >= m) clampedFj = m - 1;

                                sum += lineBuffer[ki][clampedFj] * C[ki][kj];
                            }
                        }

                        newLine[j] = sum;
                    }

                    // Write result back to F
                    System.arraycopy(newLine, 0, F[i], 0, m);
                }

            });

            threads[t].start();
            startRow = endRow;
        }

        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}