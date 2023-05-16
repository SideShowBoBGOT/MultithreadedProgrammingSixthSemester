package LabLogic;

import java.awt.*;
import java.util.Random;

public class BallRandomFactory {
    private final Random random = new Random();
    public BallRandomFactory() {}
    public Ball createBall(LabLogic.Map map) {
        var ball = new Ball(map);

    }

    private void updateBallLocation(Ball ball) {
        var mapSize = ball.map.getSize();
        var ballSize = ball.getSize();

        var rand = new Random();

        var x = rand.nextInt(mapSize.x - ballSize.x);
        var y = rand.nextInt(mapSize.y - ballSize.y);
        ball.setLocation(new Point(x, y));
    }

    private void updateBallVelocity(Ball ball) {
        var v =
    }
}
