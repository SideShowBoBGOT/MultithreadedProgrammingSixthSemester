package LabLogic;

import java.awt.*;
import java.util.Random;

public class BallFactory {
    private static final double EPSILON = 0.0001;
    private final Random random = new Random();
    private BallFactoryPrams params;

    public static class BallFactoryPrams implements Cloneable {
        public double minVelocityPercentage = 0;
        public double maxVelocityPercentage = 0;
        public double radiusPercentage = 0;
        public double redColorPercentage = 0;
        public double blueColorPercentage = 0;
        public double greenColorPercentage = 0;
        public BallMap ballMap;

        @Override
        public BallFactoryPrams clone() {
            try {
                return (BallFactoryPrams) super.clone();
            } catch (CloneNotSupportedException exception) {
                exception.printStackTrace();
            }
            return null;
        }
    }

    public BallFactory() {}

    public void setParams(BallFactoryPrams inParams) {
        params = inParams.clone();
        checkParams();
    }

    public Ball createBall(BallMap ballMap) {
        checkParams();
        var ball = new Ball(ballMap);
        updateBallRadius(ball);
        updateBallLocation(ball);
        updateBallVelocity(ball);
        updateBallColor(ball);
        return ball;
    }

    private void checkParams() {
        assert params.radiusPercentage >= 0 && params.radiusPercentage <= 1;
        assert params.minVelocityPercentage >= 0 && params.minVelocityPercentage <= 1;
        assert params.maxVelocityPercentage >= 0 && params.maxVelocityPercentage <= 1;
        assert params.minVelocityPercentage <= params.maxVelocityPercentage;
        assert params.redColorPercentage >= 0 && params.redColorPercentage <= 1;
        assert params.blueColorPercentage >= 0 && params.blueColorPercentage <= 1;
        assert params.greenColorPercentage >= 0 && params.greenColorPercentage <= 1;
        var colorProbSum = params.redColorPercentage + params.blueColorPercentage + params.greenColorPercentage;
        assert Math.abs(colorProbSum - 1) < EPSILON;
    }

    private void updateBallRadius(Ball ball) {
        var mapSize = ball.ballMap.getSize();
        var magnitude = Math.sqrt(Math.pow(mapSize.x, 2) + Math.pow(mapSize.y, 2));
        var radius = (int)(params.radiusPercentage * magnitude);
        ball.setRadius(radius);
    }

    private void updateBallLocation(Ball ball) {
        var mapSize = ball.ballMap.getSize();
        var ballRadius = ball.getRadius();

        var x = random.nextInt(mapSize.x - ballRadius);
        var y = random.nextInt(mapSize.y - ballRadius);

        ball.setLocation(new Point(x, y));
    }

    private void updateBallVelocity(Ball ball) {
        var mapSize = ball.ballMap.getSize();

        var minValX = (int)(mapSize.x * params.minVelocityPercentage);
        var maxValX = (int)(mapSize.x * params.maxVelocityPercentage);
        var minValY = (int)(mapSize.y * params.minVelocityPercentage);
        var maxValY = (int)(mapSize.y * params.maxVelocityPercentage);

        var vX = minValX + random.nextInt(maxValX - minValX);
        var vY = minValY + random.nextInt(maxValY - minValY);

        ball.setVelocity(new Point(vX, vY));
    }

    private void updateBallColor(Ball ball) {
        var randomVal = random.nextDouble();

        if(randomVal < params.redColorPercentage) {
            ball.setColor(Color.Red);
            return;
        }

        if(randomVal < (params.redColorPercentage + params.blueColorPercentage)) {
            ball.setColor(Color.Blue);
            return;
        }

        ball.setColor(Color.Green);
    }
}
