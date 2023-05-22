package BilliardsGamePart.LabThreads;

import BilliardsGamePart.Models.MapObject;
import BilliardsGamePart.Models.ObjectType;

public class BallThreadFactory {
    private static final String ERROR_NOT_BALL_TYPE = "Map object is not ball type";
    public BallThreadFactory() {}

    public BallThread createBallThread(MapObject ball) {
        assert ball.getType().equals(ObjectType.Ball) : ERROR_NOT_BALL_TYPE;
        BallThread thread = new BallThread(ball);
        updatePriority(thread, ball);
        return thread;
    }

    public void updatePriority(BallThread thread, MapObject ball) {
        var priority = 0;
        switch(ball.getColor()) {
            case Red -> priority = Thread.MAX_PRIORITY;
            default -> priority = Thread.MIN_PRIORITY;
        }
        thread.setPriority(priority);
    }

}
