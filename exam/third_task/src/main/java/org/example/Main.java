package org.example;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

public class Main {

    private static class MaxLengthTask extends RecursiveTask<Integer> {
        private static final int THRESHOLD = 100;
        private final List<String> words;
        private final int start;
        private final int end;

        MaxLengthTask(final List<String> words, final int start, final int end) {
            this.words = words;
            this.start = start;
            this.end = end;
        }

        @Override
        protected Integer compute() {
            if (end - start <= THRESHOLD) {
                return computeDirectly();
            } else {
                int mid = (start + end) / 2;
                final var rhs = new MaxLengthTask(words, mid, end);
                rhs.fork();
                final var lhs = new MaxLengthTask(words, start, mid);

                final var lhsRes = lhs.compute();
                final var rhsRes = rhs.join();
                return Math.max(lhsRes, rhsRes);
            }
        }

        private int computeDirectly() {
            var maxLen = 0;
            for(int i = start; i < end; i++) {
                maxLen = Math.max(words.get(i).length(), maxLen);
            }
            return maxLen;
        }
    }

    public static void main(String[] args) throws IOException {
        var pool = Executors.newFixedThreadPool(10);

        ExecutorService

        String[] fileNames = {"1.txt", "2.txt"};
        List<String> all_words = new ArrayList<>();
        for(var fileName : fileNames) {
            final var words = Files.readAllLines(Paths.get(fileName));
            all_words.addAll(words);
        }
        final var pool = ForkJoinPool.commonPool();
        final var maxLength = pool.invoke(new MaxLengthTask(all_words, 0, all_words.size()));
        System.out.println("Max word length: " + maxLength);
    }

}

//Max word length: 18
//
//Process finished with exit code 0