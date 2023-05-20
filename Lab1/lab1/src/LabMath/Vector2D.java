package LabMaths;

public class Vector2D implements MathVector<Vector2D> {
    private final ArgumentsVector vec = new ArgumentsVector();
    private double x;
    private double y;

    public Vector2D(double x, double y) {
        vec.setArguments(new double[]{x, y});
    }

    public double getX() {
        return getAt(Coords.X.ordinal());
    }

    public double getY() {
        return getAt(Coords.Y.ordinal());
    }

    public void setX(double value) {
        setAt(Coords.X.ordinal(), value);
    }

    public void setY(double value) {
        setAt(Coords.Y.ordinal(), value);
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
    public double size() {
        return vec.size();
    }

    @Override
    public double sizeSquared() {
        return vec.sizeSquared();
    }

    @Override
    public double dotProduct(Vector2D other) {
        return vec.dotProduct(other.vec);
    }

    @Override
    public double distance(Vector2D other) {
        return vec.distance(other.vec);
    }

    @Override
    public double getAt(int index) {
        return vec.getAt(index);
    }

    @Override
    public void setAt(int index, double value) {
        vec.setAt(index, value);
    }
}
