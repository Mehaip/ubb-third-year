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



    public static void main(String[] args){
        System.out.println("PPD - Tema1\n");

  
        int n = 1000;
        int m = 1000;
        int k = 5;
        int[][] F = new int[n][m];
        int[][] V = new int[n][m];
        int[][] C = new int[k][k];

        FileManager fm;
        try {
            fm = new FileManager();
            int[][] matFile = generateMatrix(n, m, 10);
            int[][] mat_convFile = generateMatrix(k, k, 10);
            fm.writeInputFile(matFile, n, m, mat_convFile, k);
            fm.readFromFile(n,m,k,F,C);

            long start = System.currentTimeMillis();
            ConvolutionStrategy convolution = new SequentialStrategy();
            convolution.applyConvolution(F, V, C, n, m, k);
            long end = System.currentTimeMillis();

            fm.writeOutputFile(V, n, m);
            System.out.println("Sequential: " + (end - start) + " ms\n");

            start = System.currentTimeMillis();
            ConvolutionStrategy horizontal = new HorizontalStrategy(16);
            horizontal.applyConvolution(matFile, V, mat_convFile, n, m, k);
            end = System.currentTimeMillis();

            System.out.println("Horizontal: " + (end - start) + " ms");

        } catch (IOException e){
            System.out.println("Error creating FileManager: " + e.getMessage());
            e.printStackTrace();
        }

    
        
        
    }
    

}
