package Exercise;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class SharedCollectionExample {
    private static final int N = 5;
    private static final int ITERATIONS = 1000;
    private static final List<String> sharedCollection = new ArrayList<>();

    public static void main(String[] args) {
        List<Thread> threads = new ArrayList<>();

        for (int i = 0; i < N; i++) {
            Thread thread = new Thread(new StringProducer(), "Thread " + (i + 1));
            threads.add(thread);
            thread.start();
        }

        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("Total strings in shared collection: " + sharedCollection.size());
    }

    static class StringProducer implements Runnable {
        private static final String CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        private static final int MAX_LENGTH = 10;

        @Override
        public void run() {
            Random random = new Random();

            for (int i = 0; i < ITERATIONS; i++) {
                String randomString = generateRandomString(random);
                addToSharedCollection(randomString);
            }
        }

        private String generateRandomString(Random random) {
            int length = random.nextInt(MAX_LENGTH) + 1;
            StringBuilder sb = new StringBuilder(length);

            for (int i = 0; i < length; i++) {
                int index = random.nextInt(CHARACTERS.length());
                char randomChar = CHARACTERS.charAt(index);
                sb.append(randomChar);
            }

            return sb.toString();
        }

        private void addToSharedCollection(String string) {
            synchronized (sharedCollection) {
                sharedCollection.add(string);
            }
        }
    }
}
