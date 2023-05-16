package LabVisuals;

import LabLogic.Ball;

import java.awt.*;
import java.awt.geom.Ellipse2D;


public class BallDrawer {
    public static void draw(Graphics2D g, Ball ball) {
        g.setColor(determineColor(ball));
        var loc = ball.getLocation();
        var size = ball.getSize();
        g.fill(new Ellipse2D.Double(loc.x, loc.y, size.x, size.y));
    }
    private static Color determineColor(Ball ball) {
        return switch(ball.getColor()) {
            case Red -> Color.red;
            case Blue -> Color.blue;
            case Green -> Color.green;
        };
    }

}
