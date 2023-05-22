package CounterPart.DesynchronyzedVersion;

public class DesynchronizedCounter {
    private static final int STEP = 100000;
    private static final int WAIT_TIME = 100;
    private int counter = 0;

    public static void main(String[] args) throws InterruptedException {
        var desynchronizedCounter = new DesynchronizedCounter();

        var thread1 = new Thread(desynchronizedCounter::increment);
        var thread2 = new Thread(desynchronizedCounter::decrement);

        thread1.start();
        thread2.start();

        thread1.join();
        thread2.join();
    }

    public void increment() {
        while(true) {
            counter += STEP;
            System.out.println(counter);
            try {
                Thread.sleep(WAIT_TIME);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void decrement() {
        while(true) {
            counter -= STEP;
            System.out.println(counter);
            try {
                Thread.sleep(WAIT_TIME);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
