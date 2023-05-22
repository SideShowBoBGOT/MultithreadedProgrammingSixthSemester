package BilliardsGamePart.Models;

import LabUsefulStuff.LabMath.Vector2D;

import java.util.Random;

public class MapObjectFactory {
    private final Random random = new Random();
    private Params params;

    public static class Params implements Cloneable {
        public double minVelocityPercentage = 0;
        public double maxVelocityPercentage = 0;
        public double radiusPercentage = 0;
        public final Vector2D straightLocation = new Vector2D();
        public final Vector2D straightVelocity = new Vector2D();

        @Override
        public Params clone() {
            try {
                return (Params) super.clone();
            } catch (CloneNotSupportedException exception) {
                exception.printStackTrace();
            }
            return null;
        }
    }

    public MapObjectFactory() {}

    public void setParams(Params inParams) {
        params = inParams.clone();
        checkParams();
    }

    public MapObject createHole() {
        checkParams();
        var mapObject = new MapObject();
        mapObject.setType(ObjectType.Hole);
        mapObject.setColor(ObjectColor.Black);
        updateRadius(mapObject);
        return mapObject;
    }

    public MapObject createBall(ObjectColor color, ObjectBehaviour behaviour) {
        checkParams();
        var mapObject = new MapObject();
        mapObject.setBehaviour(behaviour);
        mapObject.setType(ObjectType.Ball);
        mapObject.setColor(color);
        updateRadius(mapObject);
        updateLocation(mapObject);
        updateVelocity(mapObject);
        return mapObject;
    }


    private void checkParams() {
        assert params.radiusPercentage >= 0 && params.radiusPercentage <= 1;
        assert params.minVelocityPercentage >= 0 && params.minVelocityPercentage <= 1;
        assert params.maxVelocityPercentage >= 0 && params.maxVelocityPercentage <= 1;
        assert params.minVelocityPercentage <= params.maxVelocityPercentage;
    }

    private void updateRadius(MapObject mapObject) {
        var mapSize = MainMap.getInstance().getSize();
        var magnitude = Math.sqrt(Math.pow(mapSize.getX(), 2) + Math.pow(mapSize.getY(), 2));
        var radius = (int)(params.radiusPercentage * magnitude);
        mapObject.setDiam(radius);
    }

    private void updateLocation(MapObject mapObject) {
        switch(mapObject.getBehaviour()) {
            case Straight -> updateLocationStraight(mapObject);
            case Randomized -> updateLocationRandomized(mapObject);
        }
    }

    private void updateLocationStraight(MapObject mapObject) {
        mapObject.setLocation(params.straightLocation);
    }

    private void updateLocationRandomized(MapObject mapObject) {
        var mapSize = MainMap.getInstance().getSize();
        var ballRadius = mapObject.getDiam();

        var x = random.nextDouble(mapSize.getX() - ballRadius);
        var y = random.nextDouble(mapSize.getY() - ballRadius);

        mapObject.setLocation(x, y);
    }

    private void updateVelocity(MapObject mapObject) {
        switch(mapObject.getBehaviour()) {
            case Straight -> updateVelocityStraight(mapObject);
            case Randomized -> updateVelocityRandomized(mapObject);
        }
    }

    private void updateVelocityStraight(MapObject mapObject) {
        mapObject.setVelocity(params.straightVelocity);
    }

    private void updateVelocityRandomized(MapObject mapObject) {
        var mapSize = MainMap.getInstance().getSize();

        var minValX = mapSize.getX() * params.minVelocityPercentage;
        var maxValX = mapSize.getX() * params.maxVelocityPercentage;
        var minValY = mapSize.getY() * params.minVelocityPercentage;
        var maxValY = mapSize.getY() * params.maxVelocityPercentage;

        var vX = minValX + random.nextDouble(maxValX - minValX);
        var vY = minValY + random.nextDouble(maxValY - minValY);

        mapObject.setVelocity(vX, vY);
    }
}
