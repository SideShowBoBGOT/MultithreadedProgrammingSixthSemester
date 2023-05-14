package LabLogic;

import java.util.ArrayList;

public class Map implements Cloneable {
    private Point size = new Point();
    private ArrayList<Ball> balls = new ArrayList<>();

    public Map(Point inSize) {
        size = inSize.clone();
    }
    public Point GetSize() { return (Point) size.clone(); }
    public ArrayList<Ball> GetBalls() { return (ArrayList<Ball>) balls.clone(); }
    public void AddBall(Ball ball) { balls.add(ball); }
    public void RemoveBall(Ball ball) { balls.remove(ball); }
    @Override
    public Map clone() {
        try {
            return (Map) super.clone();
        } catch(CloneNotSupportedException e) {
            throw new AssertionError();
        }
    }
}
