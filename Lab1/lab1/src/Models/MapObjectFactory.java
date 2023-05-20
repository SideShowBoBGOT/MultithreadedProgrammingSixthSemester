package Models;

import java.util.Random;

public class MapObjectFactory {
    private static final double EPSILON = 0.0001;
    private final Random random = new Random();
    private Params params;

    public static class Params implements Cloneable {
        public double minVelocityPercentage = 0;
        public double maxVelocityPercentage = 0;
        public double radiusPercentage = 0;

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
        updateRadius(mapObject);
        return mapObject;
    }

    public MapObject createBall(ObjectColor color, BallCreationType creationType) {
        checkParams();
        var mapObject = new MapObject();
        mapObject.setType(ObjectType.Ball);
        updateRadius(mapObject);
        updateLocation(mapObject, creationType);
        updateVelocity(mapObject, creationType);
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
        mapObject.setRadius(radius);
    }

    private void updateLocation(MapObject mapObject, BallCreationType creationType) {
        var mapSize = MainMap.getInstance().getSize();
        var ballRadius = mapObject.getRadius();

        var x = random.nextDouble(mapSize.getX() - ballRadius);
        var y = random.nextDouble(mapSize.getY() - ballRadius);

        mapObject.setLocation(x, y);
    }

    private void updateLocationStraight(MapObject mapObject) {

    }

    private void updateLocationRandom(MapObject mapObject) {

    }

    private void updateVelocity(MapObject mapObject, BallCreationType creationType) {
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
