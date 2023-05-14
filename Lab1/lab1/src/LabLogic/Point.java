package LabLogic;

import java.lang.Cloneable;

public class Point implements Cloneable {
    private double x;
    private double y;
    static public double DistanceBetween(Point first, Point second) {
        var delta = first.clone();
        delta.Sub(second);
        return delta.Size();
    }
    public Point() {}
    public Point(double x, double y) {
        this.x = x;
        this.y = y;
    }
    public void SetX(double inX) { x = inX; }
    public void SetY(double inY) { y = inY; }
    public double GetX() { return x; }
    public double GetY() { return y; }
    public void Add(Point p) {
        x += p.x;
        y += p.y;
    }
    public void Sub(Point p) {
        x -= p.x;
        y -= p.y;
    }
    public double Size() {
        return Math.sqrt(this.x * this.x + this.y * this.y);
    }

    @Override
    public Point clone() {
        try {
            return (Point) super.clone();
        } catch(CloneNotSupportedException e) {
            throw new AssertionError();
        }
    }
}
