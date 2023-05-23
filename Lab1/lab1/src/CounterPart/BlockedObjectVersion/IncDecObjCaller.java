package CounterPart.BlockedObjectVersion;

public class IncDecObjCaller implements Runnable {
    private IncDecObj incDecObj;
    private boolean isInc = true;

    public IncDecObjCaller(IncDecObj incDecObj, boolean isInc) {
        this.incDecObj = incDecObj;
        this.isInc = isInc;
    }

    public void run() {
        while(true) {
            if(isInc) {
                incDecObj.increment();
                continue;
            }
            incDecObj.decrement();
        }
    }
}
