package LabMath;

public class Vector2D implements MathVector<Vector2D> {
    private final ArgumentsVector vec = new ArgumentsVector(2);

    public Vector2D() {}

    public Vector2D(double x, double y) {
        set(x, y);
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

    public void set(double x, double y) {
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
        return new Vector2D(getX(), getY());
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
    public void sub(Vector2D other) {
        vec.sub(other.vec);
    }

    @Override
    public void multiply(Vector2D other) {
        vec.multiply(other.vec);
    }

    @Override
    public void divide(Vector2D other) {
        vec.divide(other.vec);
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
