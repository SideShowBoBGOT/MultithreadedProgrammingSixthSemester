package LabVisuals;

import LabLogic.Ball;
import LabLogic.BallFactory;
import java.awt.*;
import java.awt.geom.Ellipse2D;


public class BallDrawer {
    public static void draw(Graphics2D g, Ball ball) {
        g.setColor(determineColor(ball));
        var loc = ball.getLocation();
        var radius = ball.getRadius();
        g.fill(new Ellipse2D.Double(loc.getX(), loc.getY(), radius, radius));
    }
    private static Color determineColor(Ball ball) {
        return switch(ball.getColor()) {
            case Red -> Color.red;
            case Blue -> Color.blue;
            case Green -> Color.green;
        };
    }

}
