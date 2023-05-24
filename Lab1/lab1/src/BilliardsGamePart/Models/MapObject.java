package BilliardsGamePart.Models;

import LabUsefulStuff.LabMath.Coords;
import LabUsefulStuff.LabMath.Vector2D;

import java.util.Random;

public class MapObject {
    private static final double EPSILON = 0.01;
    private static final String AssertionErrorNotPositiveMessage = "Value is not positive";

    private final Vector2D location = new Vector2D();
    private final Vector2D velocity = new Vector2D();
    private double diam = 0;
    private ObjectBehaviour behaviour = ObjectBehaviour.Straight;
    private ObjectColor color = ObjectColor.Blue;
    private ObjectType type = ObjectType.Ball;

    public MapObject() {
        MainMap.getInstance().addMapObject(this);
    }

    public synchronized void setLocation(Vector2D inLocation) {
        location.set(inLocation);
    }

    public synchronized void setLocation(double x, double y) {
        location.set(x, y);
    }
    public synchronized void setLocationAt(Coords coords, double value) {
        location.setAt(coords.ordinal(), value);
    }

    public synchronized Vector2D getLocation() { return location.clone(); }
    public synchronized double getLocationAt(Coords coords) { return location.getAt(coords.ordinal()); }

    public synchronized void setDiam(int inRadius) {
        assert inRadius >= 0 : AssertionErrorNotPositiveMessage;
        diam = inRadius;
    }

    public synchronized double getDiam() { return diam; }

    public synchronized void setVelocity(Vector2D inVelocity) {
        velocity.set(inVelocity);
    }

    public synchronized void setVelocity(double x, double y) {
        velocity.set(x, y);
    }
    public synchronized void setVelocityAt(Coords coords, double value) {
        velocity.setAt(coords.ordinal(), value);
    }

    public synchronized Vector2D getVelocity() { return velocity.clone(); }
    public synchronized double getVelocityAt(Coords coords) { return velocity.getAt(coords.ordinal()); }

    public synchronized ObjectColor getColor() { return color; }

    public synchronized void setColor(ObjectColor inObjectColor) { color = inObjectColor; }

    public synchronized ObjectType getType() { return type; }

    public synchronized void setType(ObjectType inType) { type = inType; }

    public synchronized ObjectBehaviour getBehaviour() { return behaviour; }

    public synchronized void setBehaviour(ObjectBehaviour inBehaviour) { behaviour = inBehaviour; }

    public synchronized Vector2D getCenterLocation() {
        var center = new Vector2D();
        center.set(location.getX() + diam, location.getY() + diam);
        return center;
    }

    public synchronized void tick() {
        location.add(velocity);
        if(isInsideHole()) {
            MainMap.getInstance().removeMapObject(this);
        }
        bounceMap();
    }

    private synchronized void bounceMap() {
        checkBounce(Coords.X);
        checkBounce(Coords.Y);
    }

    private synchronized void rotateVelocityRandomly() {
        var forwardVec = velocity.getForwardVector();
        var deg = (new Random()).nextDouble(360);
        forwardVec.rotate(deg);
        forwardVec.multiply(velocity.getSize());
        setVelocity(forwardVec);
    }

    private synchronized boolean isPointInside(Vector2D point) {
        var dist = getCenterLocation().getDistance(point);
        var res = (dist - getDiam() / 2) < EPSILON;
        return res;
    }

    private synchronized boolean isInsideHole() {
        for(var obj : MainMap.getInstance().getBalls()) {
            if(obj.type.equals(ObjectType.Hole)) {
                var res = obj.isPointInside(getCenterLocation());
                if(res) {
                    return true;
                }
            }
        }
        return false;
    }

    private synchronized void checkBounce(Coords coords) {
        var maxCoord = MainMap.getInstance().getSize().getAt(coords.ordinal());
        var curCoord = getLocationAt(coords);
        var isBounced = false;
        if(curCoord < 0) {
            setLocationAt(coords, EPSILON);
            isBounced = true;
        } else if(curCoord + diam - maxCoord > EPSILON) {
            setLocationAt(coords, maxCoord - diam - EPSILON);
            isBounced = true;
        }
        if(isBounced) {
            switch(getBehaviour()) {
                case Straight -> setVelocityAt(coords, -getVelocityAt(coords));
                case Randomized -> rotateVelocityRandomly();
            }
        }
    }
}

