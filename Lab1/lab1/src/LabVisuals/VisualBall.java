package LabVisuals;

import LabLogic.Ball;

import java.awt.*;
import java.awt.geom.Ellipse2D;


public class VisualBall {
    private final Component canvas;
    private final Ball ball;
    public VisualBall(Component inCanvas, Ball inBall) {
        canvas = inCanvas;
        ball = inBall;
    }
    public void Draw(Graphics2D g) {
        canvas.repaint();
        g.setColor(DetermineColor());
        var loc = ball.GetLocation();
        var size = ball.GetSize();
        g.fill(new Ellipse2D.Double(loc.GetX(), loc.GetY(), size.GetX(), size.GetY()));
    }
    private Color DetermineColor() {
        switch(ball.GetColor()) {
            case Red: return Color.red;
            case Blue: return Color.blue;
            case Green: return Color.green;
        }
        return Color.green;
    }

}
