package CounterPart.BlockedObjectVersion;

public class IncDecObj {
    private static final int WAIT_TIME = 100;
    private int counter = 0;
    private int step = 100000;
    private boolean isIncrementing = true;

    IncDecObj(int step) {
        this.step = step;
    }

    synchronized void increment() {
        while(isIncrementing) doWait();
        counter += step;
        isIncrementing = true;
        printCounter();
        sleep();
        notifyAll();
    }

    synchronized void decrement() {
        while(!isIncrementing) doWait();
        counter -= step;
        isIncrementing = false;
        printCounter();
        sleep();
        notifyAll();
    }

    private void printCounter() {
        System.out.println(counter);
    }

    void doWait() {
        try {
            wait();
        } catch(InterruptedException e) {
            e.printStackTrace();
        }
    }

    void sleep() {
        try {
            Thread.sleep(WAIT_TIME);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
