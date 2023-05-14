package LabLogic;

public class Ball {
    private static final double MinDistance = 1;
    private Point location = new Point();
    private Point velocity = new Point();
    private Point size = new Point();
    private Color color = Color.Blue;
    private Map map;
    public Ball(Map map) { this.map = map; }
    public void SetLocation(Point inLocation) { location = inLocation.clone(); }
    public Point GetLocation() { return location.clone(); }
    public void SetSize(Point inSize) { size = inSize.clone(); }
    public Point GetSize() { return size.clone(); }
    public void SetVelocity(Point inVelocity) { velocity = inVelocity.clone(); }
    public Point GetVelocity() { return velocity.clone(); }
    public Color GetColor() { return color; }
    public void SetColor(Color inColor) { color = inColor; }
    public Point GetCenterLocation() {
        var center = new Point();
        center.SetX(location.GetX() + size.GetX() / 2);
        center.SetY(location.GetY() + size.GetY() / 2);
        return center;
    }
    public void Tick() {
        location.Add(velocity);
        BounceMap();
        BounceAnyBalls();
    }
    private void BounceAnyBalls() {
        var center = GetCenterLocation();
        for(Ball b : map.GetBalls()) {
            if(b==this) continue;
            var otherCenter = b.GetCenterLocation();
            var distance = Point.DistanceBetween(center, otherCenter);
            if(distance < MinDistance) {
                BounceBall();
            }
        }
    }
    private void BounceBall() {
        velocity.SetX(-velocity.GetX());
        velocity.SetY(-velocity.GetY());
    }
    private void BounceMap() {
        var mapSize = map.GetSize();
        var resX = GetBounceResult(location.GetX(), velocity.GetX(), size.GetX(), mapSize.GetX());
        var resY = GetBounceResult(location.GetY(), velocity.GetY(), size.GetY(), mapSize.GetY());
        location.SetX(resX.GetX());
        velocity.SetX(resX.GetY());
        location.SetY(resY.GetX());
        velocity.SetY(resY.GetY());
    }
    private Point GetBounceResult(int inAx, int inVelocity, int inSize, int inMaxAx) {
        var res = new Point();
        if(inAx < 0) {
            res.SetX(0);
        } else if(inAx + inSize >= inMaxAx) {
            res.SetX(inMaxAx - inSize);
        }
        res.SetY(-inVelocity);
        return res;
    }
}

