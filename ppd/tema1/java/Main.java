import java.io.IOException;
import java.util.Random;
import java.util.Scanner;

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

    public static void runTestLab2(int N, int M, int k, int[] threadCounts, String inputFileName, String outputFileName) {
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Testing with N=" + N + ", M=" + M + ", k=" + k);
        System.out.println("=".repeat(60));

        final int NUM_RUNS = 1;

        try {
            int[][] F = new int[N][M];
            int[][] C = new int[k][k];
            FileManager fm = new FileManager(inputFileName, outputFileName);


            // Read input
            fm.readFromFile(N, M, k, F, C);

            // Sequential in-place - run 10 times and average
            long sequentialTotal = 0;
            int[][] V = new int[N][M];  // Dummy V for interface compatibility

            for (int run = 0; run < NUM_RUNS; run++) {
                // Re-read F for each run since it's modified in-place
                fm.readFromFile(N, M, k, F, C);
                V = new int[N][M];
                long start = System.currentTimeMillis();
                ConvolutionStrategy sequential = new InPlaceSequentialStrategy();
                sequential.applyConvolution(F, V, C, N, M, k);
                long end = System.currentTimeMillis();
                sequentialTotal += (end - start);
            }

            double sequentialAvg = (double) sequentialTotal / NUM_RUNS;
            System.out.println("Sequential in-place (avg of " + NUM_RUNS + " runs): " +
                             String.format("%.2f", sequentialAvg) + " ms");

            // Write output file (using the last sequential run result)
            fm.writeOutputFile(F, N, M);

            // Test with different thread counts (horizontal only for Lab 2)
            for (int p : threadCounts) {
                System.out.println("\n  [p=" + p + " threads]");

                // Horizontal strategy - run 10 times and average
                long horizontalTotal = 0;
                for (int run = 0; run < NUM_RUNS; run++) {
                    // Re-read F for each run since it's modified in-place
                    fm.readFromFile(N, M, k, F, C);
                    V = new int[N][M];
                    long start = System.currentTimeMillis();
                    ConvolutionStrategy horizontal = new InPlaceHorizontalStrategy(p);
                    horizontal.applyConvolution(F, V, C, N, M, k);
                    long end = System.currentTimeMillis();
                    horizontalTotal += (end - start);
                }
                double horizontalAvg = (double) horizontalTotal / NUM_RUNS;
                double horizontalSpeedup = sequentialAvg / horizontalAvg;
                System.out.println("    Horizontal in-place (avg of " + NUM_RUNS + " runs): " +
                                 String.format("%.2f", horizontalAvg) + " ms (speedup: " +
                                 String.format("%.2f", horizontalSpeedup) + "x)");
            }

        } catch (IOException e) {
            System.out.println("Error during test: " + e.getMessage());
            e.printStackTrace();
        }
    }

    // public static void runTestLab1(int N, int M, int k, int[] threadCounts, String pathOutput) {
    //     System.out.println("\n" + "=".repeat(60));
    //     System.out.println("Testing with N=" + N + ", M=" + M + ", k=" + k);
    //     System.out.println("=".repeat(60));

    //     final int NUM_RUNS = 1;

    //     try {
    //         System.out.println("only one run");
    //         int[][] F = new int[N][M];
    //         int[][] C = new int[k][k];
    //         FileManager fm = new FileManager();

            
    //         //int[][] matFile = generateMatrix(N, M, 10);
    //         //int[][] mat_convFile = generateMatrix(k, k, 10);
    //         //fm.writeInputFile(matFile, N, M, mat_convFile, k);

            
    //         fm.readFromFile(N, M, k, F, C);

            
    //         long sequentialTotal = 0;
    //         int[][] V = new int[N][M];

    //         for (int run = 0; run < NUM_RUNS; run++) {
    //             V = new int[N][M];
    //             long start = System.currentTimeMillis();
    //             ConvolutionStrategy sequential = new SequentialStrategy();
    //             sequential.applyConvolution(F, V, C, N, M, k);
    //             long end = System.currentTimeMillis();
    //             sequentialTotal += (end - start);
    //         }

    //         double sequentialAvg = (double) sequentialTotal / NUM_RUNS;
    //         System.out.println("Sequential (avg of " + NUM_RUNS + " runs): " +
    //                          String.format("%.2f", sequentialAvg) + " ms");

 
    //         fm.writeOutputFile(V, N, M, pathOutput);

            
    //         for (int p : threadCounts) {
    //             System.out.println("\n  [p=" + p + " threads]");

                
    //             long horizontalTotal = 0;
    //             for (int run = 0; run < NUM_RUNS; run++) {
    //                 V = new int[N][M];
    //                 long start = System.currentTimeMillis();
    //                 ConvolutionStrategy horizontal = new HorizontalStrategy(p);
    //                 horizontal.applyConvolution(F, V, C, N, M, k);
    //                 long end = System.currentTimeMillis();
    //                 horizontalTotal += (end - start);
    //             }
    //             double horizontalAvg = (double) horizontalTotal / NUM_RUNS;
    //             double horizontalSpeedup = sequentialAvg / horizontalAvg;
    //             System.out.println("    Horizontal (avg of " + NUM_RUNS + " runs): " +
    //                              String.format("%.2f", horizontalAvg) + " ms (speedup: " +
    //                              String.format("%.2f", horizontalSpeedup) + "x)");

                
    //             long verticalTotal = 0;
    //             for (int run = 0; run < NUM_RUNS; run++) {
    //                 V = new int[N][M];
    //                 long start = System.currentTimeMillis();
    //                 ConvolutionStrategy vertical = new VerticalStrategy(p);
    //                 vertical.applyConvolution(F, V, C, N, M, k);
    //                 long end = System.currentTimeMillis();
    //                 verticalTotal += (end - start);
    //             }
    //             double verticalAvg = (double) verticalTotal / NUM_RUNS;
    //             double verticalSpeedup = sequentialAvg / verticalAvg;
    //             System.out.println("    Vertical (avg of " + NUM_RUNS + " runs): " +
    //                              String.format("%.2f", verticalAvg) + " ms (speedup: " +
    //                              String.format("%.2f", verticalSpeedup) + "x)");
    //         }

    //     } catch (IOException e) {
    //         System.out.println("Error during test: " + e.getMessage());
    //         e.printStackTrace();
    //     }
    // }

    public static void main(String[] args){
        Scanner scanner = new Scanner(System.in);

        System.out.println("=".repeat(60));
        System.out.println("PPD - Matrix Convolution - Lab Selection");
        System.out.println("=".repeat(60));
        System.out.println("\nSelect which lab to run:");
        System.out.println("1 - Laborator 1 (Standard convolution with separate output matrix)");
        System.out.println("2 - Laborator 2 (In-place convolution with O(n) space)");
        System.out.print("\nEnter your choice (1 or 2): ");

        int choice = scanner.nextInt();

        if (choice == 1) {
            System.out.println("\n*** Running Laborator 1 Tests ***\n");

            // Test 1: N=M=10, k=3, p=4
            //runTestLab1(10, 10, 3, new int[]{4}, "/home/mijaj/CodingTomfoolery/uni/ppd/tema1/data/output.txt");

            // Test 2: N=M=1000, k=5, p=2,4,8,16
            //runTestLab1(1000, 1000, 5, new int[]{2, 4, 8, 16},"/home/mijaj/CodingTomfoolery/uni/ppd/tema1/data/output2.txt");

            // Test 3: N=10, M=10000, k=5, p=2,4,8,16
            //runTestLab1(10, 10000, 5, new int[]{2, 4, 8, 16}, "/home/mijaj/CodingTomfoolery/uni/ppd/tema1/data/output3.txt");

            // Test 4: N=10000, M=10, k=5, p=2,4,8,16
            //runTestLab1(10000, 10, 5, new int[]{2, 4, 8, 16},"/home/mijaj/CodingTomfoolery/uni/ppd/tema1/data/output4.txt");

            // Test 5: N=10000, M=10000, k=5, p=2,4,8,16
            //runTestLab1(10000, 10000, 5, new int[]{2, 4, 8, 16},"/home/mijaj/CodingTomfoolery/uni/ppd/tema1/data/outputjava.txt");

        } else if (choice == 2) {
            System.out.println("\n*** Running Laborator 2 Tests ***\n");

            // Test 1: N=M=10, k=3, p=2
            runTestLab2(10, 10, 3, new int[]{2}, "data/matrice10.txt", "results/java/output10.txt");

            // Test 2: N=M=1000, k=3, p=2,4,8,16
            runTestLab2(1000, 1000, 3, new int[]{2, 4, 8, 16}, "data/matrice1000.txt", "results/java/output1000.txt");

            // Test 3: N=M=10000, k=3, p=2,4,8,16
            runTestLab2(10000, 10000, 3, new int[]{2, 4, 8, 16}, "data/matrice10000.txt", "results/java/output10000.txt");

        } else {
            System.out.println("Invalid choice! Please run again and select 1 or 2.");
            scanner.close();
            return;
        }

        System.out.println("\n" + "=".repeat(60));
        System.out.println("All tests completed!");
        System.out.println("=".repeat(60));

        scanner.close();
    }


}
