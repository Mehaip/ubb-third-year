package uni.ppd.lab1;

import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.CyclicBarrier;
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

        c_seq = timeArraySum("Sequential", () -> sumArraysSeq(a, b, VEC_SIZE));

        if (a.length <= 10){
            System.out.println(Arrays.toString(a));
            System.out.println(Arrays.toString(b));
            System.out.println(Arrays.toString(c_seq));
        }

        c_cycl = timeArraySum("Cyclical", () -> {
            try {
                return sumArraysCycl(a, b, VEC_SIZE, NO_THREADS);
            } catch (InterruptedException e){
                throw new RuntimeException(e);
            }
        });


    }

    public static int[] generateArray(int size, int upperBound){
        int[] vec = new int[size];
        Random rand = new Random();

        for (int i = 0; i < size; i++){
            vec[i] = rand.nextInt(upperBound);
        }
        return vec;
    }

    private static int[] sumArraysSeq(int[] A, int[] B, int size){
        int[] C = new int[size];
        for (int i = 0; i < size; i++)
            C[i] = A[i] + B[i];

        return C;
    }

    private static int[] sumArraysCycl(
        int[] A,
        int[] B,
        int size,
        int noThreads
    ) throws InterruptedException {
        int[] C = new int[size];
        CyclicalThread[] threads = new CyclicalThread[noThreads];

        for(int i = 0; i < noThreads; i++){
            threads[i] = new CyclicalThread(i, noThreads, size, A, B, C);
            threads[i].start();
        }

        for (int i = 0; i < noThreads; i++){
            threads[i].join();
        }

        return C;
    }
}