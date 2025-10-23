package uni.ppd.lab1;

import java.util.Arrays;
import java.util.Random;
import java.util.function.Supplier;

public class Main {

    private static int[] timeArraySum(
        String label,
        Supplier<int[]> sumFunction
    ) {
        long startTime = System.currentTimeMillis();
        int[] result = sumFunction.get();
        long endTime = System.currentTimeMillis();
        System.out.println(label + " time: " + (endTime - startTime) + " ms");
        return result;
    }

    public static void main(String[] args){
        System.out.println("PPD - Lab1");
        
        int VEC_SIZE = 10000000;
        int NO_THREADS = 4;

        int[] a = generateArray(VEC_SIZE,10);
        int[] b = generateArray(VEC_SIZE, 10);
        int[] c_seq, c_cycl, c_int;


    }

    public static int[] generateArray(int size, int upperBound){
        int[] vec = new int[size];
        Random rand = new Random();

        for (int i = 0; i < size; i++){
            vec[i] = rand.nextInt(upperBound);
        }
        return vec;
    }
}