package CounterPart.SynchronizedBlockVersion;

import java.util.Vector;

public class SynchronizedBlock {
    public static void main(String[] args) {
        var incDec = new IncDec(100000);

        int totalIncrementingThreads = 6;
        int totalDecrementingThreads = 10;
        var threads = new Vector<Thread>();

        for(int i = 0; i < totalIncrementingThreads; i++) {
            threads.add(new Thread(new CallerIncDec(incDec, true)));
        }

        for(int i = 0; i < totalDecrementingThreads; i++) {
            threads.add(new Thread(new CallerIncDec(incDec, false)));
        }

        for(var thread : threads) {
            thread.start();
        }

        for(var thread : threads) {
            try {
                thread.join();
            } catch(InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
