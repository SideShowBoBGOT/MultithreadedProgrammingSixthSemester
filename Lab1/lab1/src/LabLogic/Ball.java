package LabLogic;

import java.awt.Point;

public class Ball {
    private static final double MinDistance = 1;
    private static final String AssertionErrorNotPositiveMessage = "Value is not positive";

    private Point location = new Point();
    private Point velocity = new Point();
    private int radius = 0;
    private Color color = Color.Blue;
    public final BallMap ballMap;

    public Ball(BallMap ballMap) {
        this.ballMap = ballMap;
        ballMap.addBall(this);
    }

    public void setLocation(Point inLocation) {
        assert inLocation.x >= 0 && inLocation.y >= 0 : AssertionErrorNotPositiveMessage;
        location = (Point) inLocation.clone();
    }

    public Point getLocation() { return (Point) location.clone(); }

    public void setRadius(int inRadius) {
        assert inRadius >= 0 : AssertionErrorNotPositiveMessage;
        radius = inRadius;
    }

    public int getRadius() { return radius; }

    public void setVelocity(Point inVelocity) {
        assert inVelocity.x >= 0 && inVelocity.y >= 0 : AssertionErrorNotPositiveMessage;
        velocity = (Point) inVelocity.clone();
    }

    public Point getVelocity() { return (Point) velocity.clone(); }

    public Color getColor() { return color; }

    public void setColor(Color inColor) { color = inColor; }

    public Point getCenterLocation() {
        var center = new Point();
        center.setLocation(location.x + radius, location.y + radius);
        return center;
    }

    public void tick() {
        location.move(velocity.x, velocity.y);
        bounceMap();
        bounceAnyBalls();
    }

    private void bounceAnyBalls() {
        var center = getCenterLocation();
        for(Ball b : ballMap.getBalls()) {
            if(b==this) continue;
            var otherCenter = b.getCenterLocation();
            var distance = center.distance(otherCenter);
            if(distance < MinDistance) {
                bounceBall();
            }
        }
    }

    private void bounceBall() {
        velocity.setLocation(-velocity.x, -velocity.y);
    }

    private void bounceMap() {
        var mapSize = ballMap.getSize();
        var resX = getBounceResult(location.x, velocity.x, mapSize.x);
        var resY = getBounceResult(location.y, velocity.y, mapSize.y);
        location.setLocation(resX.x, resY.x);
        velocity.setLocation(resX.y, resY.y);
    }

    private Point getBounceResult(double inAx, double inVelocity, double inMaxAx) {
        var res = new Point();
        if(inAx < 0) {
            res.x = 0;
        } else if(inAx + radius >= inMaxAx) {
            res.x = (int)(inMaxAx - radius);
        }
        res.y = (int)(-inVelocity);
        return res;
    }
}

