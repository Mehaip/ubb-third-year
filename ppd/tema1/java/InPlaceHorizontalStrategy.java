public class InPlaceHorizontalStrategy implements ConvolutionStrategy {
    private final int numThreads;

    public InPlaceHorizontalStrategy(int numThreads) {
        this.numThreads = numThreads;
    }

    private static class WorkerThread extends Thread {
        private final int[][] F;
        private final int[][] C;
        private final int n, m, k;
        private final int startRow, endRow;
        private final int[][] sharedRowBuffers;
        private final Object[] locks;

        public WorkerThread(int[][] F, int[][] C, int n, int m, int k,
                          int startRow, int endRow, int[][] sharedRowBuffers, Object[] locks) {
            this.F = F;
            this.C = C;
            this.n = n;
            this.m = m;
            this.k = k;
            this.startRow = startRow;
            this.endRow = endRow;
            this.sharedRowBuffers = sharedRowBuffers;
            this.locks = locks;
        }

        @Override
        public void run() {
            int halfK = k / 2;

            // Each thread processes its assigned rows
            for (int i = startRow; i < endRow; i++) {
                // Read the rows we need (with boundary clamping)
                int[][] localBuffers = new int[k][m];

                for (int ki = 0; ki < k; ki++) {
                    int rowIdx = i + ki - halfK;
                    if (rowIdx < 0) rowIdx = 0;
                    if (rowIdx >= n) rowIdx = n - 1;

                    // Check if we need to use shared buffer or can read directly
                    // We need shared buffer only for rows that might be modified by other threads
                    if (rowIdx >= startRow && rowIdx < endRow) {
                        // This row is in our range, but might be modified
                        // Use shared buffer if it exists
                        synchronized (locks[rowIdx]) {
                            if (sharedRowBuffers[rowIdx] != null) {
                                System.arraycopy(sharedRowBuffers[rowIdx], 0, localBuffers[ki], 0, m);
                            } else {
                                System.arraycopy(F[rowIdx], 0, localBuffers[ki], 0, m);
                            }
                        }
                    } else {
                        // Row is outside our range, safe to read directly
                        synchronized (locks[rowIdx]) {
                            if (sharedRowBuffers[rowIdx] != null) {
                                System.arraycopy(sharedRowBuffers[rowIdx], 0, localBuffers[ki], 0, m);
                            } else {
                                System.arraycopy(F[rowIdx], 0, localBuffers[ki], 0, m);
                            }
                        }
                    }
                }

                // Compute convolution for this row
                int[] resultRow = new int[m];
                for (int j = 0; j < m; j++) {
                    int sum = 0;

                    for (int ki = 0; ki < k; ki++) {
                        for (int kj = 0; kj < k; kj++) {
                            int fj = j + kj - halfK;

                            // Clamp column index
                            if (fj < 0) fj = 0;
                            if (fj >= m) fj = m - 1;

                            sum += localBuffers[ki][fj] * C[ki][kj];
                        }
                    }

                    resultRow[j] = sum;
                }

                // Write result back to F[i]
                synchronized (locks[i]) {
                    System.arraycopy(resultRow, 0, F[i], 0, m);
                }
            }
        }
    }

    @Override
    public void applyConvolution(int[][] F, int[][] V, int[][] C, int n, int m, int k) {
        int halfK = k / 2;

        // Create shared row buffers for boundary rows that might be accessed by multiple threads
        int[][] sharedRowBuffers = new int[n][];
        Object[] locks = new Object[n];

        for (int i = 0; i < n; i++) {
            locks[i] = new Object();
            // Save original rows before any modifications
            sharedRowBuffers[i] = new int[m];
            System.arraycopy(F[i], 0, sharedRowBuffers[i], 0, m);
        }

        // Create threads
        Thread[] threads = new Thread[numThreads];
        int rowsPerThread = n / numThreads;
        int extraRows = n % numThreads;

        int startRow = 0;
        for (int t = 0; t < numThreads; t++) {
            int endRow = startRow + rowsPerThread + (t < extraRows ? 1 : 0);

            threads[t] = new WorkerThread(F, C, n, m, k, startRow, endRow, sharedRowBuffers, locks);
            threads[t].start();

            startRow = endRow;
        }

        // Wait for all threads
        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // V is not used in Lab 2, but copy F to V for interface compatibility
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                V[i][j] = F[i][j];
            }
        }
    }
}
