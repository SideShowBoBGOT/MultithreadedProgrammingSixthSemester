package CounterPart.BlockedObjectVersion;

import java.util.Vector;

public class BlockedObject {
    public static void main(String[] args) {
        var step = 100000;
        IncDecObj incDecObj = new IncDecObj(step);


        int totalIncrementingThreads = 6;
        int totalDecrementingThreads = 10;
        var threads = new Vector<Thread>();

        for(int i = 0; i < totalIncrementingThreads; i++) {
            threads.add(new Thread(new IncDecObjCaller(incDecObj, true)));
        }

        for(int i = 0; i < totalDecrementingThreads; i++) {
            threads.add(new Thread(new IncDecObjCaller(incDecObj, false)));
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
