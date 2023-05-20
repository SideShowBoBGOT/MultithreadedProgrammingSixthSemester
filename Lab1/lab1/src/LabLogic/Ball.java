package LabLogic;

import LabMath.Vector2D;

public class Ball {
    private static final double MinDistance = 1;
    private static final String AssertionErrorNotPositiveMessage = "Value is not positive";

    private final Vector2D location = new Vector2D();
    private final Vector2D velocity = new Vector2D();
    private int radius = 0;
    private BallColor ballColor = BallColor.Blue;

    public Ball() {
        BallMap.getInstance().addBall(this);
    }

    public void setLocation(Vector2D inLocation) {
        location.set(inLocation);
    }

    public void setLocation(double x, double y) {
        location.set(x, y);
    }

    public Vector2D getLocation() { return location.clone(); }

    public void setRadius(int inRadius) {
        assert inRadius >= 0 : AssertionErrorNotPositiveMessage;
        radius = inRadius;
    }

    public int getRadius() { return radius; }

    public void setVelocity(Vector2D inVelocity) {
        velocity.set(inVelocity);
    }

    public void setVelocity(double x, double y) {
        velocity.set(x, y);
    }

    public Vector2D getVelocity() { return (Vector2D) velocity.clone(); }

    public BallColor getColor() { return ballColor; }

    public void setColor(BallColor inBallColor) { ballColor = inBallColor; }

    public Vector2D getCenterLocation() {
        var center = new Vector2D();
        center.set(location.getX() + radius, location.getY() + radius);
        return center;
    }

    public void tick() {
        location.add(velocity);
        bounceMap();
        bounceAnyBalls();
    }

    private void bounceAnyBalls() {
        var center = getCenterLocation();
        for(Ball b : BallMap.getInstance().getBalls()) {
            if(b==this) continue;
            var otherCenter = b.getCenterLocation();
            var distance = center.getDistance(otherCenter);
            if(distance < MinDistance) {
                bounceBall();
            }
        }
    }

    private void bounceBall() {
        velocity.toOpposite();
    }

    private void bounceMap() {
        var mapSize = BallMap.getInstance().getSize();
        var resX = getBounceResult(
                location.getX(), velocity.getX(), mapSize.getX());
        var resY = getBounceResult(
                location.getY(), velocity.getY(), mapSize.getY());
        location.set(resX.getX(), resY.getX());
        velocity.set(resX.getY(), resY.getY());
    }

    private Vector2D getBounceResult(double inAx, double inVelocity, double inMaxAx) {
        var res = new Vector2D();
        if(inAx < 0) {
            res.setX(0);
        } else if(inAx + radius >= inMaxAx) {
            res.setX(inMaxAx - radius);
        }
        res.setY(-inVelocity);
        return res;
    }
}

