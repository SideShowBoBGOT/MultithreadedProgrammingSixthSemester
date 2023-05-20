package LabLogic;

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

    public Ball createBall() {
        checkParams();
        var ball = new Ball();
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
        var mapSize = BallMap.getInstance().getSize();
        var magnitude = Math.sqrt(Math.pow(mapSize.getX(), 2) + Math.pow(mapSize.getY(), 2));
        var radius = (int)(params.radiusPercentage * magnitude);
        ball.setRadius(radius);
    }

    private void updateBallLocation(Ball ball) {
        var mapSize = BallMap.getInstance().getSize();
        var ballRadius = ball.getRadius();

        var x = random.nextDouble(mapSize.getX() - ballRadius);
        var y = random.nextDouble(mapSize.getY() - ballRadius);

        ball.setLocation(x, y);
    }

    private void updateBallVelocity(Ball ball) {
        var mapSize = BallMap.getInstance().getSize();

        var minValX = mapSize.getX() * params.minVelocityPercentage;
        var maxValX = mapSize.getX() * params.maxVelocityPercentage;
        var minValY = mapSize.getY() * params.minVelocityPercentage;
        var maxValY = mapSize.getY() * params.maxVelocityPercentage;

        var vX = minValX + random.nextDouble(maxValX - minValX);
        var vY = minValY + random.nextDouble(maxValY - minValY);

        ball.setVelocity(vX, vY);
    }

    private void updateBallColor(Ball ball) {
        var randomVal = random.nextDouble();

        if(randomVal < params.redColorPercentage) {
            ball.setColor(BallColor.Red);
            return;
        }

        if(randomVal < (params.redColorPercentage + params.blueColorPercentage)) {
            ball.setColor(BallColor.Blue);
            return;
        }

        ball.setColor(BallColor.Green);
    }
}
