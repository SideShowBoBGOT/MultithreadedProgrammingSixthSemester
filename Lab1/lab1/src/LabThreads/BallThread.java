package LabThreads;

import Models.MapObject;

import java.util.HashMap;

public class BallThread extends Thread {

    private static final String THREAD_ID_LOG = "ThreadID";
    private static final String LOCATION_LOG = "Location";
    private static final String VELOCITY_LOG = "Velocity";
    private static final String COLOR_LOG = "Color";

    private final HashMap<String, Object> logHash = new HashMap<>();
    private String logString = "";
    private final StringBuilder stringBuilder = new StringBuilder(4);
    private final MapObject mapObject;

    public BallThread(MapObject inMapObject) {
        mapObject = inMapObject;
        initLogHash();
    }

    private void initLogHash() {
        logHash.put(THREAD_ID_LOG, threadId());
        logHash.put(LOCATION_LOG, null);
        logHash.put(VELOCITY_LOG, null);
        logHash.put(COLOR_LOG, null);
    }

    private void updateLogHash() {
        logHash.replace(LOCATION_LOG, mapObject.getLocation());
        logHash.replace(VELOCITY_LOG, mapObject.getVelocity());
        logHash.replace(COLOR_LOG, mapObject.getColor());
    }

    @Override
    public void run() {
        try {
            while(true) {
                mapObject.tick();
                updateLogHash();
                logBall();
                Thread.sleep(17);
            }
        } catch(InterruptedException ignored) {
        }
    }

    private void logBall() {
        logString = "";
        logHash.forEach(this::doLogBall);
        System.out.println(logString);
    }

    private void doLogBall(String key, Object value) {
        logString += "[" + key + ": " + value.toString() + "]\t";
    }
}