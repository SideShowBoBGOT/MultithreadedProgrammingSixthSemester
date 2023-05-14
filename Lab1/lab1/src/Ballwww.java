import LabLogic.Point;

import java.awt.*;
import java.awt.geom.Ellipse2D;
import java.util.Random;


public class Ballwww {
    private final Component canvas;
    private static final Point Size = new Point(20, 20);
    private Point velocity = new Point();
    private Point location = new Point();
    public Ballwww(Component c) {
        this.canvas = c;
        var rand = new Random();
        location.x = rand.nextInt(canvas.getWidth() - Size.x);
        location.y = rand.nextInt(canvas.getHeight() - Size.y);
    }
    public void Draw(Graphics2D g2) {
        g2.setColor(Color.darkGray);
        g2.fill(new Ellipse2D.Double(location.x, location.y, Size.x, Size.y));
    }
    public void BounceWalls() {
        if(location.x < 0) {

        }
    }
    public void UpdateLocation() {
        location.x += velocity.x;
        location.y += velocity.y;
        if(x < 0) {
            x = 0;
            dx = -dx;
        }
        if(x + XSIZE >= this.canvas.getWidth()) {
            x = this.canvas.getWidth() - XSIZE;
            dx = -dx;
        }
        if(y < 0) {
            y = 0;
            dy = -dy;
        }
        if(y + YSIZE >= this.canvas.getHeight()) {
            y = this.canvas.getHeight() - YSIZE;
            dy = -dy;
        }
        this.canvas.repaint();
    }
}
