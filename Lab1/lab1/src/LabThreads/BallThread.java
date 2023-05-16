package LabThreads;

import LabLogic.Ball;

import java.awt.*;
import java.util.Random;

public class BallThread extends Thread {
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
        } catch(InterruptedException ex) {
        }
    }



    private void logBall() {
        StringBuilder log = new StringBuilder();
        Object[] logs = new Object[]{getName(), ball.getLocation(), ball.getVelocity(), ball.getColor()};
        for(var l : logs) log.append(decorateLog(l)).append("\t");
        System.out.println(log);
    }

    private String decorateLog(Object obj) {
        return "[" + obj.toString() + "]";
    }
}