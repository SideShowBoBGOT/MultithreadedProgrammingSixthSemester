package LabUsefulStuff.LabMath;

public interface MathVector<T> {
    void add(T other);
    void sub(T other);
    void multiply(T other);
    void multiply(double value);
    void divide(T other);
    void divide(double value);
    double getSize();
    double getSizeSquared();
    double getDotProduct(T other);
    double getDistance(T other);
    T getForwardVector();
    double getAt(int index);
    void setAt(int index, double value);
    T getOpposite();
    void toOpposite();
    void set(T other);
}
