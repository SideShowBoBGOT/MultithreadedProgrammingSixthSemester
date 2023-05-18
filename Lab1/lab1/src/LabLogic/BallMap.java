package LabLogic;

import java.util.ArrayList;
import java.awt.Point;

public class BallMap implements Cloneable {
    private Point size = new Point();
    private ArrayList<Ball> balls = new ArrayList<>();

    public BallMap(Point inSize) {
        size = (Point) inSize.clone();
    }
    public Point getSize() { return (Point) size.clone(); }
    public ArrayList<Ball> getBalls() { return (ArrayList<Ball>) balls.clone(); }
    public void addBall(Ball ball) { balls.add(ball); }
    public void removeBall(Ball ball) { balls.remove(ball); }
    @Override
    public BallMap clone() {
        try {
            return (BallMap) super.clone();
        } catch(CloneNotSupportedException e) {
            throw new AssertionError();
        }
    }
}
