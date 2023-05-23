package CounterPart.SynchronizedMethodsVersion;

import java.util.Vector;

public class SynchronizedCounter {
    private static final int STEP = 100000;
    private static final int WAIT_TIME = 100;

    private static boolean semaphore = true;
    private static int counter = 0;

    public static void main(String[] args) throws InterruptedException {
        int totalIncrementingThreads = 2;
        int totalDecrementingThreads = 3;
        var threads = new Vector<Thread>();

        for(int i = 0; i < totalIncrementingThreads; i++) {
            threads.add(new Thread(() -> doWork(true)));
        }

        for(int i = 0; i < totalDecrementingThreads; i++) {
            threads.add(new Thread(() -> doWork(false)));
        }

        for(var thread : threads) {
            thread.start();
        }

        for(var thread : threads) {
            thread.join();
        }
    }

    private static void doWork(boolean isIncrementing) {
        while(true) {
            incDec(isIncrementing);
        }
    }

    private static synchronized void incDec(boolean isIncrementing) {
        if(isIncrementing && semaphore) {
            counter += STEP;
            System.out.println(counter);
            semaphore = false;
            sleep();
        } else if(!isIncrementing && !semaphore) {
            counter -= STEP;
            System.out.println(counter);
            semaphore = true;
            sleep();
        }
    }

    private static void sleep() {
        try {
            Thread.sleep(WAIT_TIME);
        } catch(InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
