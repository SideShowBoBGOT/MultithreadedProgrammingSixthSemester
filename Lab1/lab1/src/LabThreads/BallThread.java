package LabThreads;

import LabLogic.Ball;
import LabLogic.BallColor;
import LabMath.Vector2D;

import java.awt.*;
import java.util.Random;

public class BallThread extends Thread {

    private static class BallLog {
        private static final String LOCATION_LOG = "Location";
        private static final String VELOCITY_LOG = "Velocity";
        private static final String COLOR_LOG = "Color";
        private

        BallLog(Ball ball) {}
    }

    private final Ball ball;

    public BallThread(Ball inBall) {
        ball = inBall;
    }

    @Override
    public void run() {
        try {
            while(true) {
                ball.tick();
                logBall();
                Thread.sleep(17);
            }
        } catch(InterruptedException ignored) {
        }
    }

    private void logBall() {
        StringBuilder log = new StringBuilder();
        Object[] logs = new Object[]{getName(), ball.getLocation(), ball.getVelocity(), ball.getColor()};
        for(var l : logs) log.append(decorateLog(l)).append(" ");
        System.out.println(log);
    }

    private String decorateLog(Object obj) {
        return "[" + obj.toString() + "]";
    }
}