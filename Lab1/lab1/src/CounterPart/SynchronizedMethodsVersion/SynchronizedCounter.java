package CounterPart.SynchronizedMethodsVersion;

public class SynchronizedCounter {
    private static final int STEP = 100000;
    private static final int WAIT_TIME = 100;
    private boolean isIncrementing = true;
    private int counter = 0;

    public static void main(String[] args) throws InterruptedException {
        var synchronizedCounter = new SynchronizedCounter();

        var thread1 = new Thread(synchronizedCounter::increment);
        var thread2 = new Thread(synchronizedCounter::decrement);

        thread1.start();
        thread2.start();

        thread1.join();
        thread2.join();
    }

    private void
}
