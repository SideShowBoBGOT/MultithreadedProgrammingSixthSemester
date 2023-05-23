package BilliardsGamePart.LabThreads;

import BilliardsGamePart.Models.MapObject;

import java.util.HashMap;

public class BallThreadLog {
    private static final String THREAD_ID_LOG = "ThreadID";
    private static final String LOCATION_LOG = "Location";
    private static final String VELOCITY_LOG = "Velocity";
    private static final String COLOR_LOG = "Color";
    private static final int LOG_CAPACITY = 200;
    private static final int LOG_HASH_CAPACITY = 4;

    private final HashMap<String, Object> logHash = new HashMap<>(LOG_HASH_CAPACITY);
    private final StringBuilder stringBuilder = new StringBuilder(LOG_CAPACITY);

    private final MapObject mapObject;

    BallThreadLog(BallThread thread, MapObject inMapObject) {
        mapObject = inMapObject;
        logHash.put(THREAD_ID_LOG, thread.threadId());
        logHash.put(LOCATION_LOG, mapObject.getLocation());
        logHash.put(VELOCITY_LOG, mapObject.getVelocity());
        logHash.put(COLOR_LOG, mapObject.getColor());
    }

    public void print() {
        update();
        stringBuilder.setLength(0);
        for(var entry : logHash.entrySet()) {
            stringBuilder.append("[").append(entry.getKey()).append(": ")
                    .append(entry.getValue().toString()).append("]\t");
        }
        System.out.println(stringBuilder.toString());
    }

    public void informEnded() {
        System.out.println("[THREAD_ID_LOG: " + logHash.get(THREAD_ID_LOG) + "]\t[Status: ENDED]");
    }

    private void update() {
        logHash.replace(LOCATION_LOG, mapObject.getLocation());
        logHash.replace(VELOCITY_LOG, mapObject.getVelocity());
        logHash.replace(COLOR_LOG, mapObject.getColor());
    }
}
