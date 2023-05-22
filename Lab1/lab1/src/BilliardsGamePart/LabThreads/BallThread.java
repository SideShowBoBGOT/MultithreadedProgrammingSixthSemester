package BilliardsGamePart.LabThreads;

import BilliardsGamePart.LabControllers.Application;
import BilliardsGamePart.Models.MainMap;
import BilliardsGamePart.Models.MapObject;

import java.beans.PropertyChangeSupport;

public class BallThread extends Thread {
    private static final String FINISHED = "Thread finished: ";
    private static final String STATE_PROPERTY = "State";
    private static final int SLEEP_TIME = 10;
    private final MapObject mapObject;
    private final BallThreadLog log;
    private PropertyChangeSupport support;

    public BallThread(MapObject inMapObject) {
        mapObject = inMapObject;
        log = new BallThreadLog(this, mapObject);
        support = new PropertyChangeSupport(this);
        support.addPropertyChangeListener(Application.getInstance());
    }

    @Override
    public void run() {
        try {
            while(true) {
                mapObject.tick();
                log.print();
                checkValid();
                Thread.sleep(SLEEP_TIME);
            }
        } catch(InterruptedException ignored) {
        }
    }

    private void checkValid() throws InterruptedException {
        if(MainMap.getInstance().contains(mapObject)) return;
        support.firePropertyChange(STATE_PROPERTY, State.RUNNABLE, State.WAITING);
        join();
    }

}