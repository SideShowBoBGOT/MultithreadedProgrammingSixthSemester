package CounterPart.SynchronizedBlockVersion;

public class IncDec {
    private static final int SLEEP_TIME = 100;

    private int step = 100000;
    private int counter = 0;
    private boolean isIncOrDec = true;

    IncDec(int step) {
        this.step = step;
    }

    public void increment() {
        if(!isIncOrDec) return;
        isIncOrDec = false;
        counter += step;
        printCounter();
        sleep();
    }

    public void decrement() {
        if(isIncOrDec) return;
        isIncOrDec = true;
        counter -= step;
        printCounter();
        sleep();
    }

    private void printCounter() {
        System.out.println(counter);
    }

    private void sleep() {
        try {
            Thread.sleep(SLEEP_TIME);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
