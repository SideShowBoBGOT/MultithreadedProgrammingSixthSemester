package LabVisuals;

import LabLogic.Ball;

import java.awt.*;
import java.awt.geom.Ellipse2D;


public class BallDrawer {
    public static void Draw(Graphics2D g, Ball ball) {
        g.setColor(DetermineColor(ball));
        var loc = ball.GetLocation();
        var size = ball.GetSize();
        g.fill(new Ellipse2D.Double(loc.GetX(), loc.GetY(), size.GetX(), size.GetY()));
    }
    private static Color DetermineColor(Ball ball) {
        return switch(ball.GetColor()) {
            case Red -> Color.red;
            case Blue -> Color.blue;
            case Green -> Color.green;
        };
    }

}
