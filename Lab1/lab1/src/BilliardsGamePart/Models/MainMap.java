package BilliardsGamePart.Models;

import java.util.ArrayList;

import LabUsefulStuff.LabMath.Vector2D;

public class MainMap {

    private final Vector2D size = new Vector2D();
    private final ArrayList<MapObject> mapObjects = new ArrayList<>();
    private final ArrayList<MapObject> holes = new ArrayList<>();
    private static MainMap instance;

    private MainMap() {}

    public synchronized static MainMap getInstance() {
        if(instance == null) {
            instance = new MainMap();
        }
        return instance;
    }

    public synchronized void setSize(Vector2D inSize) {
        size.set(inSize);
    }

    public synchronized void setSize(double x, double y) {
        size.set(x, y);
    }

    public synchronized Vector2D getSize() {
        return size.clone();
    }

    public synchronized ArrayList<MapObject> getBalls() {
        return (ArrayList<MapObject>) mapObjects.clone();
    }

    public synchronized void addMapObject(MapObject mapObject) {
        mapObjects.add(mapObject);
    }

    public synchronized void removeMapObject(MapObject mapObject) {
        mapObjects.remove(mapObject);
    }

    public synchronized boolean contains(MapObject mapObject) {
        return mapObjects.contains(mapObject);
    }
}
