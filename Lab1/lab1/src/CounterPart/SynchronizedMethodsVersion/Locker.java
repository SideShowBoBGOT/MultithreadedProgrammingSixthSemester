package CounterPart.SynchronizedMethodsVersion;

public class Locker {
    private boolean lock = true;

    public synchronized void lock() {
        while(lock);
        lock = true;
    }

    public synchronized void unlock() {
        while(!lock);
        lock = false;
    }

    public synchronized boolean isLocked() {
        return lock;
    }
}
