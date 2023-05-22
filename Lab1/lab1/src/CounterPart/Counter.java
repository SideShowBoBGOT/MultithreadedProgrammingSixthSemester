package CounterPart;

public class Counter {
    private static final int STEP = 100000;
    private int counter = 0;

    public synchronized void increment() {
        counter += STEP;
    }

    public synchronized void decrement() {
        counter -= STEP;
    }

    public synchronized int getCounter() {
        return counter;
    }
}
