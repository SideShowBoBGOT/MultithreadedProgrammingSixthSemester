package LabMaths;

public interface MathVector<T> {
    void add(T other);
    void sub(T other);
    void multiply(T other);
    void divide(T other);
    double size();
    double sizeSquared();
    double dotProduct(T other);
    double distance(T other);
    double getAt(int index);
    void setAt(int index, double value);
}
