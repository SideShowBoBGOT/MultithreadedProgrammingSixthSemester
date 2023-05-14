package LabLogic;

import java.lang.Cloneable;

public class Point implements Cloneable {
    private int x = 0;
    private int y = 0;
    static public double DistanceBetween(Point first, Point second) {
        var delta = first.clone();
        delta.Sub(second);
        return delta.Size();
    }
    public Point() {}
    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }
    public void SetX(int inX) { x = inX; }
    public void SetY(int inY) { y = inY; }
    public int GetX() { return x; }
    public int GetY() { return y; }
    public void Add(Point p) {
        this.x += p.x;
        this.y += p.y;
    }
    public void Sub(Point p) {
        this.x -= p.x;
        this.y -= p.y;
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
