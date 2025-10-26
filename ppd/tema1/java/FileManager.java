package tema1.java;
import java.io.File;
import java.io.IOException;
import java.nio.file.*;
import java.io.FileWriter;

public class FileManager {
    File inputFile = new File("tema1/java/data/date.txt");
    File outputFile = new File("tema1/java/data/output.txt");
    
    FileManager() throws IOException {
        Files.newOutputStream(inputFile.toPath(), 
            StandardOpenOption.CREATE, 
            StandardOpenOption.TRUNCATE_EXISTING).close();
        
        Files.newOutputStream(outputFile.toPath(), 
            StandardOpenOption.CREATE, 
            StandardOpenOption.TRUNCATE_EXISTING).close();
        
    }



    void writeInputFile(int mat[][], int n, int m, int mat_conv[][], int k)
        throws IOException{

        FileWriter writer = new FileWriter("tema1/java/data/date.txt");
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                writer.write(Integer.toString(mat[i][j]));
                writer.write(" ");
            }
            writer.write("\n");

        }
        for(int i = 0; i < k; i++){
            for(int j = 0; j < k; j++){
                writer.write(Integer.toString(mat_conv[i][j]));
                writer.write(" ");
            }
            writer.write("\n");

        }



        writer.close();

    }
}