package Exercise;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.Callable;

public class ParallelSumExample {
    private static final int ARRAY_SIZE = 1000000;
    private static final int NUM_TASKS = 100;
    private static final int THREAD_POOL_SIZE = 8;

    public static void main(String[] args) {
        double[] array = generateRandomArray(ARRAY_SIZE);

        ExecutorService executorService = Executors.newFixedThreadPool(THREAD_POOL_SIZE);
        Future<Double>[] futures = new Future[NUM_TASKS];

        int chunkSize = ARRAY_SIZE / NUM_TASKS;

        for (int i = 0; i < NUM_TASKS; i++) {
            int startIndex = i * chunkSize;
            int endIndex = (i == NUM_TASKS - 1) ? ARRAY_SIZE : (i + 1) * chunkSize;
            futures[i] = executorService.submit(new SumTask(array, startIndex, endIndex));
        }

        double totalSum = 0;

        for (Future<Double> future : futures) {
            try {
                totalSum += future.get();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        executorService.shutdown();

        System.out.println("Total sum: " + totalSum);
    }

    static class SumTask implements Callable<Double> {
        private final double[] array;
        private final int startIndex;
        private final int endIndex;

        public SumTask(double[] array, int startIndex, int endIndex) {
            this.array = array;
            this.startIndex = startIndex;
            this.endIndex = endIndex;
        }

        @Override
        public Double call() {
            double partialSum = 0;

            for (int i = startIndex; i < endIndex; i++) {
                partialSum += array[i];
            }

            return partialSum;
        }
    }

    private static double[] generateRandomArray(int size) {
        double[] array = new double[size];

        for (int i = 0; i < size; i++) {
            array[i] = Math.random();
        }

        return array;
    }
}