package CounterPart.SynchronizedBlockVersion;

public class CallerIncDec implements Runnable {
    private final IncDec incDec;
    private boolean isInc = true;

    CallerIncDec(IncDec incDec, boolean isInc) {
        this.incDec = incDec;
        this.isInc = isInc;
    }


    @Override
    public void run() {
        while(true) {
            synchronized(incDec) {
                if(isInc) {
                    incDec.increment();
                } else {
                    incDec.decrement();
                }
            }
        }
    }
}
