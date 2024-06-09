import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

public class AverageWordLength extends RecursiveTask<Double> {
    private static final int THRESHOLD = 1000;
    private List<String> words;
    private int start;
    private int end;

    public AverageWordLength(List<String> words, int start, int end) {
        this.words = words;
        this.start = start;
        this.end = end;
    }

    @Override
    protected Double compute() {
        if (end - start <= THRESHOLD) {
            int totalLength = 0;
            for (int i = start; i < end; i++) {
                totalLength += words.get(i).length();
            }
            return (double) totalLength / (end - start);
        } else {
            int mid = (start + end) / 2;
            AverageWordLength left = new AverageWordLength(words, start, mid);
            AverageWordLength right = new AverageWordLength(words, mid, end);
            left.fork();
            double rightResult = right.compute();
            double leftResult = left.join();
            return (leftResult + rightResult) / 2;
        }
    }

    public static void main(String[] args) {
        List<String> wordList = new ArrayList<>();

        wordList.add("Kolyba");
        wordList.add("Druzhba");
        wordList.add("KPI ONE LOVE");
        wordList.add("Panchenko");
        wordList.add("IP11");

        ForkJoinPool pool = new ForkJoinPool();
        AverageWordLength task = new AverageWordLength(wordList, 0, wordList.size());
        double averageLength = pool.invoke(task);

        System.out.println("Average word length: " + averageLength);
    }
}


