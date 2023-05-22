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

    public void setLocation(Vector2D inLocation) {
        location.set(inLocation);
    }

    public void setLocation(double x, double y) {
        location.set(x, y);
    }
    public void setLocationAt(Coords coords, double value) {
        location.setAt(coords.ordinal(), value);
    }

    public Vector2D getLocation() { return location.clone(); }
    public double getLocationAt(Coords coords) { return location.getAt(coords.ordinal()); }

    public void setDiam(int inRadius) {
        assert inRadius >= 0 : AssertionErrorNotPositiveMessage;
        diam = inRadius;
    }

    public double getDiam() { return diam; }

    public void setVelocity(Vector2D inVelocity) {
        velocity.set(inVelocity);
    }

    public void setVelocity(double x, double y) {
        velocity.set(x, y);
    }
    public void setVelocityAt(Coords coords, double value) {
        velocity.setAt(coords.ordinal(), value);
    }

    public Vector2D getVelocity() { return velocity.clone(); }
    public double getVelocityAt(Coords coords) { return velocity.getAt(coords.ordinal()); }

    public ObjectColor getColor() { return color; }

    public void setColor(ObjectColor inObjectColor) { color = inObjectColor; }

    public ObjectType getType() { return type; }

    public void setType(ObjectType inType) { type = inType; }

    public ObjectBehaviour getBehaviour() { return behaviour; }

    public void setBehaviour(ObjectBehaviour inBehaviour) { behaviour = inBehaviour; }

    public Vector2D getCenterLocation() {
        var center = new Vector2D();
        center.set(location.getX() + diam, location.getY() + diam);
        return center;
    }

    public void tick() {
        location.add(velocity);
        if(isInsideHole()) {
            MainMap.getInstance().removeMapObject(this);
        }
        bounceMap();
    }

    private void bounceMap() {
        checkBounce(Coords.X);
        checkBounce(Coords.Y);
    }

    private void rotateVelocityRandomly() {
        var forwardVec = velocity.getForwardVector();
        var deg = (new Random()).nextDouble(360);
        forwardVec.rotate(deg);
        forwardVec.multiply(velocity.getSize());
        velocity.set(forwardVec);
    }

    private boolean isPointInside(Vector2D point) {
        var dist = getCenterLocation().getDistance(point);
        var res = (dist - getDiam() / 2) < EPSILON;
        return res;
    }

    private boolean isInsideHole() {
        for(var obj : MainMap.getInstance().getBalls()) {
            if(obj.getType().equals(ObjectType.Hole)) {
                var res = obj.isPointInside(getCenterLocation());
                if(res) {
                    return true;
                }
            }
        }
        return false;
    }

    private void checkBounce(Coords coords) {
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

