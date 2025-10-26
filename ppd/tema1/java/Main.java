import java.io.IOException;
import java.util.Random;

public class Main {

    public static int[][] generateMatrix(int n, int m, int upperBound){
        int[][] mat = new int[n][m];

        Random rand = new Random();

        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++)
                mat[i][j] = rand.nextInt(upperBound);
        }

        return mat;
    }    



    public static void runTest(int N, int M, int k, int[] threadCounts) {
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Testing with N=" + N + ", M=" + M + ", k=" + k);
        System.out.println("=".repeat(60));

        final int NUM_RUNS = 10;

        try {
            int[][] F = new int[N][M];
            int[][] C = new int[k][k];
            FileManager fm = new FileManager();

            // Generate and write input file
            int[][] matFile = generateMatrix(N, M, 10);
            int[][] mat_convFile = generateMatrix(k, k, 10);
            fm.writeInputFile(matFile, N, M, mat_convFile, k);

            // Read from file
            fm.readFromFile(N, M, k, F, C);

            // Sequential (no threads) - run 10 times and average
            long sequentialTotal = 0;
            int[][] V = new int[N][M];

            for (int run = 0; run < NUM_RUNS; run++) {
                V = new int[N][M];
                long start = System.currentTimeMillis();
                ConvolutionStrategy sequential = new SequentialStrategy();
                sequential.applyConvolution(F, V, C, N, M, k);
                long end = System.currentTimeMillis();
                sequentialTotal += (end - start);
            }

            double sequentialAvg = (double) sequentialTotal / NUM_RUNS;
            System.out.println("Sequential (avg of " + NUM_RUNS + " runs): " +
                             String.format("%.2f", sequentialAvg) + " ms");

            // Write output file once (for verification)
            fm.writeOutputFile(V, N, M);

            // Test with different thread counts
            for (int p : threadCounts) {
                System.out.println("\n  [p=" + p + " threads]");

                // Horizontal strategy - run 10 times and average
                long horizontalTotal = 0;
                for (int run = 0; run < NUM_RUNS; run++) {
                    V = new int[N][M];
                    long start = System.currentTimeMillis();
                    ConvolutionStrategy horizontal = new HorizontalStrategy(p);
                    horizontal.applyConvolution(F, V, C, N, M, k);
                    long end = System.currentTimeMillis();
                    horizontalTotal += (end - start);
                }
                double horizontalAvg = (double) horizontalTotal / NUM_RUNS;
                double horizontalSpeedup = sequentialAvg / horizontalAvg;
                System.out.println("    Horizontal (avg of " + NUM_RUNS + " runs): " +
                                 String.format("%.2f", horizontalAvg) + " ms (speedup: " +
                                 String.format("%.2f", horizontalSpeedup) + "x)");

                // Vertical strategy - run 10 times and average
                long verticalTotal = 0;
                for (int run = 0; run < NUM_RUNS; run++) {
                    V = new int[N][M];
                    long start = System.currentTimeMillis();
                    ConvolutionStrategy vertical = new VerticalStrategy(p);
                    vertical.applyConvolution(F, V, C, N, M, k);
                    long end = System.currentTimeMillis();
                    verticalTotal += (end - start);
                }
                double verticalAvg = (double) verticalTotal / NUM_RUNS;
                double verticalSpeedup = sequentialAvg / verticalAvg;
                System.out.println("    Vertical (avg of " + NUM_RUNS + " runs): " +
                                 String.format("%.2f", verticalAvg) + " ms (speedup: " +
                                 String.format("%.2f", verticalSpeedup) + "x)");
            }

        } catch (IOException e) {
            System.out.println("Error during test: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public static void main(String[] args){
        System.out.println("PPD - Tema1 - Comprehensive Testing\n");

        // Test 1: N=M=10, k=3, p=4
        runTest(10, 10, 3, new int[]{4});

        // Test 2: N=M=1000, k=5, p=2,4,8,16
        runTest(1000, 1000, 5, new int[]{2, 4, 8, 16});

        // Test 3: N=10, M=10000, k=5, p=2,4,8,16
        runTest(10, 10000, 5, new int[]{2, 4, 8, 16});

        // Test 4: N=10000, M=10, k=5, p=2,4,8,16
        runTest(10000, 10, 5, new int[]{2, 4, 8, 16});

        // Test 5: N=10000, M=10000, k=5, p=2,4,8,16
        runTest(10000, 10000, 5, new int[]{2, 4, 8, 16});

        System.out.println("\n" + "=".repeat(60));
        System.out.println("All tests completed!");
        System.out.println("=".repeat(60));
    }
    

}
