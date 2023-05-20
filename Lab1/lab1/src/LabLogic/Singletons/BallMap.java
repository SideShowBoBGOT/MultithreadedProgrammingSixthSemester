package LabLogic;

import java.util.ArrayList;
import LabMath.Vector2D;

public class BallMap implements Cloneable {
    private final Vector2D size = new Vector2D();
    private final ArrayList<Ball> balls = new ArrayList<>();
    private static BallMap instance;

    private BallMap() {}

    public static BallMap getInstance() {
        if(instance == null) {
          instance = new BallMap();
        }
        return instance;
    }

    public void setSize(Vector2D inSize) {
        size.set(inSize);
    }

    public void setSize(double x, double y) {
        size.set(x, y);
    }

    public Vector2D getSize() {
        return size.clone();
    }

    public ArrayList<Ball> getBalls() {
        return (ArrayList<Ball>) balls.clone();
    }

    public void addBall(Ball ball) {
        balls.add(ball);
    }

    public void removeBall(Ball ball) {
        balls.remove(ball);
    }
}
