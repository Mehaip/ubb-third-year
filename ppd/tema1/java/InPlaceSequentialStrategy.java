public class InPlaceSequentialStrategy implements ConvolutionStrategy {

    private int getElement(int[][] F, int i, int j, int n, int m) {
        if (i < 0) i = 0;
        if (i >= n) i = n - 1;
        if (j < 0) j = 0;
        if (j >= m) j = m - 1;
        return F[i][j];
    }

    @Override
    public void applyConvolution(int[][] F, int[][] V, int[][] C, int n, int m, int k) {
        // Lab 2: In-place convolution with O(n) space complexity
        // For k=3, we process row by row with a sliding window of row buffers

        int halfK = k / 2;

        // We need k row buffers to hold original values while we process
        int[][] rowBuffers = new int[k][m];

        // Initialize the buffers with the first k rows (with clamping for boundaries)
        for (int bufIdx = 0; bufIdx < k; bufIdx++) {
            int rowIdx = bufIdx - halfK;  // For k=3, this is -1, 0, 1
            if (rowIdx < 0) rowIdx = 0;
            if (rowIdx >= n) rowIdx = n - 1;

            for (int j = 0; j < m; j++) {
                rowBuffers[bufIdx][j] = F[rowIdx][j];
            }
        }

        // Process each row
        for (int i = 0; i < n; i++) {
            // Calculate result for row i using buffered values
            int[] resultRow = new int[m];

            for (int j = 0; j < m; j++) {
                int sum = 0;

                for (int ki = 0; ki < k; ki++) {
                    for (int kj = 0; kj < k; kj++) {
                        int fj = j + kj - halfK;

                        // Clamp column index
                        if (fj < 0) fj = 0;
                        if (fj >= m) fj = m - 1;

                        sum += rowBuffers[ki][fj] * C[ki][kj];
                    }
                }

                resultRow[j] = sum;
            }

            // Write result back to F[i]
            for (int j = 0; j < m; j++) {
                F[i][j] = resultRow[j];
            }

            // Shift buffers: move everything up one position
            // and load the next row into the last buffer position
            if (i < n - 1) {
                // Rotate buffers
                int[] temp = rowBuffers[0];
                for (int bufIdx = 0; bufIdx < k - 1; bufIdx++) {
                    rowBuffers[bufIdx] = rowBuffers[bufIdx + 1];
                }
                rowBuffers[k - 1] = temp;

                // Load next row (row i + halfK + 1)
                int nextRowIdx = i + halfK + 1;
                if (nextRowIdx >= n) nextRowIdx = n - 1;

                for (int j = 0; j < m; j++) {
                    rowBuffers[k - 1][j] = F[nextRowIdx][j];
                }
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
