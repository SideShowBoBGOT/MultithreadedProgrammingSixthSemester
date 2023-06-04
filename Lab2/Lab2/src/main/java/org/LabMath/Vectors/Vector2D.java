package org.LabMath.Vectors;

import org.LabMath.Enums.*;
import org.LabMath.Interfaces.MathVector;

public class Vector2D implements MathVector<Vector2D> {
    private final GeneralVector vec = new GeneralVector(2);

    public Vector2D() {}

    public Vector2D(double y, double x) {
        set(y, x);
    }

    public Vector2D(Vector2D other) {
        set(other);
    }

    public double getX() {
        return getAt(Coords.X.ordinal());
    }

    public double getY() {
        return getAt(Coords.Y.ordinal());
    }

    public void set(Vector2D other) {
        vec.set(other.vec);
    }

    public void set(double y, double x) {
        setX(x);
        setY(y);
    }

    public void setX(double value) {
        setAt(Coords.X.ordinal(), value);
    }

    public void setY(double value) {
        setAt(Coords.Y.ordinal(), value);
    }

    @Override
    public Vector2D clone() {
        return new Vector2D(getY(), getX());
    }

    @Override
    public String toString() {
        return vec.toString();
    }

    @Override
    public void add(Vector2D other) {
        vec.add(other.vec);
    }

    @Override
    public void add(double value) {
        vec.add(value);
    }

    @Override
    public void sub(double value) {
        vec.sub(value);
    }

    @Override
    public void sub(Vector2D other) {
        vec.sub(other.vec);
    }

    @Override
    public void mul(Vector2D other) {
        vec.mul(other.vec);
    }

    @Override
    public void mul(double value) {
        vec.mul(value);
    }

    @Override
    public void div(Vector2D other) {
        vec.div(other.vec);
    }

    @Override
    public void div(double value) {
        vec.div(value);
    }

    @Override
    public double getSize() {
        return vec.getSize();
    }

    @Override
    public double getSizeSquared() {
        return vec.getSizeSquared();
    }

    @Override
    public double getDotProduct(Vector2D other) {
        return vec.getDotProduct(other.vec);
    }

    @Override
    public double getDistance(Vector2D other) {
        return vec.getDistance(other.vec);
    }

    @Override
    public Vector2D getForwardVector() {
        var forwardVec = clone();
        forwardVec.vec.set(forwardVec.vec.getForwardVector());
        return forwardVec;
    }

    @Override
    public double getAt(int index) {
        return vec.getAt(index);
    }

    @Override
    public void setAt(int index, double value) {
        vec.setAt(index, value);
    }

    @Override
    public Vector2D getOpposite() {
        var v = clone();
        v.toOpposite();
        return v;
    }

    @Override
    public void toOpposite() {
        vec.toOpposite();
    }
}
