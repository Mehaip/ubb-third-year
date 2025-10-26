package tema1.java;
import java.io.File;
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

        int N = 10;
        int M = 10;
        int K = 3;
        FileManager fm;
        try {
            fm = new FileManager();
            int[][] mat = generateMatrix(N, M, 10);
            int[][] mat_conv = generateMatrix(K, K, 10);
            fm.writeInputFile(mat, N, M, mat_conv, K);
        } catch (IOException e){
            System.out.println("Error creating FileManager: " + e.getMessage());
            e.printStackTrace();
        }

        
        
    }
    

}
