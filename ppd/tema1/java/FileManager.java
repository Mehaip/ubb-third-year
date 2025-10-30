import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.*;
import java.io.FileWriter;
import java.util.Scanner;

public class FileManager {
    String inputFile;
    String outputFile;
    
    FileManager(String inputFile, String outputFile) throws IOException {
        this.inputFile = inputFile;
        this.outputFile = outputFile;

    }


    void writeOutputFile(int mat[][], int n, int m) throws IOException{
        FileWriter writer = new FileWriter(outputFile);
        writer.write(n + " " + m + "\n");
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                writer.write(Integer.toString(mat[i][j]));
                writer.write(" ");
            }
            writer.write("\n");

        }
        writer.close();
    }

    void readFromFile(int n, int m, int k, int F[][], int C[][]) throws IOException{
                try {
            Scanner scanner = new Scanner(new File(inputFile));
            
 
            n = scanner.nextInt();
            m = scanner.nextInt();
            k = scanner.nextInt();
            
            
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    F[i][j] = scanner.nextInt();
                }
            }
            

            for (int i = 0; i < k; i++) {
                for (int j = 0; j < k; j++) {
                    C[i][j] = scanner.nextInt();
                }
            }
            
            scanner.close();
            

            } catch (FileNotFoundException e) {
            System.out.println("File not found: " + e.getMessage());
        }
    }
}