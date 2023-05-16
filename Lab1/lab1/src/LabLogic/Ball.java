package LabLogic;

import java.awt.Point;

public class Ball {
    private static final double MinDistance = 1;
    private Point location = new Point();
    private Point velocity = new Point();
    private Point size = new Point();
    private Color color = Color.Blue;
    public final Map map;

    public Ball(Map map) {
        this.map = map;
        map.addBall(this);
    }

    public void setLocation(Point inLocation) { location = (Point) inLocation.clone(); }

    public Point getLocation() { return (Point) location.clone(); }

    public void setSize(Point inSize) { size = (Point) inSize.clone(); }

    public Point getSize() { return (Point) size.clone(); }

    public void setVelocity(Point inVelocity) { velocity = (Point) inVelocity.clone(); }

    public Point getVelocity() { return (Point) velocity.clone(); }

    public Color getColor() { return color; }

    public void setColor(Color inColor) { color = inColor; }

    public Point getCenterLocation() {
        var center = new Point();
        center.setLocation(location.x + size.x / 2, location.y + size.y / 2);
        return center;
    }

    public void tick() {
        location.move(velocity.x, velocity.y);
        bounceMap();
        bounceAnyBalls();
    }

    private void bounceAnyBalls() {
        var center = getCenterLocation();
        for(Ball b : map.getBalls()) {
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
        var mapSize = map.getSize();
        var resX = getBounceResult(location.x, velocity.x, size.x, mapSize.x);
        var resY = getBounceResult(location.y, velocity.y, size.y, mapSize.y);
        location.setLocation(resX.x, resY.x);
        velocity.setLocation(resX.y, resY.y);
    }

    private Point getBounceResult(double inAx, double inVelocity, double inSize, double inMaxAx) {
        var res = new Point();
        if(inAx < 0) {
            res.x = 0;
        } else if(inAx + inSize >= inMaxAx) {
            res.x = (int)(inMaxAx - inSize);
        }
        res.y = (int)(-inVelocity);
        return res;
    }
}

