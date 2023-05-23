package BilliardsGamePart.LabVisuals;

import BilliardsGamePart.Models.MapObject;

import java.awt.*;
import java.awt.geom.Ellipse2D;


public class MapObjectDrawer {
    public static void draw(Graphics2D g, MapObject mapObject) {
        g.setColor(determineColor(mapObject));
        var loc = mapObject.getLocation();
        var diam = mapObject.getDiam();
        g.fill(new Ellipse2D.Double(loc.getX(), loc.getY(), diam, diam));
    }
    private static Color determineColor(MapObject mapObject) {
        return switch(mapObject.getColor()) {
            case Red -> Color.red;
            case Blue -> Color.blue;
            case Black -> Color.black;
        };
    }

}