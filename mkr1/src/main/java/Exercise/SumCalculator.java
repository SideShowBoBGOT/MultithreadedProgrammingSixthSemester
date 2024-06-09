import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

public class SumCalculator extends RecursiveTask<Double> {
    private static final long serialVersionUID = 1L;
    private static final int THRESHOLD = 1000;

    private final double[] array;
    private final int start;
    private final int end;

    public SumCalculator(double[] array, int start, int end) {
        this.array = array;
        this.start = start;
        this.end = end;
    }

    @Override
    protected Double compute() {
        if (end - start <= THRESHOLD) {
            double sum = 0;
            for (int i = start; i < end; i++) {
                sum += array[i];
            }
            return sum;
        } else {
            int mid = (start + end) / 2;
            SumCalculator left = new SumCalculator(array, start, mid);
            SumCalculator right = new SumCalculator(array, mid, end);
            left.fork();
            double rightResult = right.compute();
            double leftResult = left.join();
            return leftResult + rightResult;
        }
    }

    public static void main(String[] args) {
        final int SIZE = 10000;
        double[] numbers = new double[SIZE];

        for (int i = 0; i < SIZE; i++) {
            numbers[i] = Math.random();
        }

        ForkJoinPool pool = new ForkJoinPool(10);
        SumCalculator calculator = new SumCalculator(numbers, 0, SIZE);
        double sum = pool.invoke(calculator);

        System.out.println("Sum: " + sum);
    }
}