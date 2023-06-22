package org.example.LabMath.Interfaces;

import org.LabMath.Interfaces.General.*;

public interface MathVector<T> extends Cloneable, Divisible<T>, Multipliable<T>, Addable<T>, Subtractable<T>,
        DoubleDivisible, DoubleMultipliable, DoubleAddable, DoubleSubtractable {
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
