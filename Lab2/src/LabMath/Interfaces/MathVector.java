package LabMath.Interfaces;

import LabMath.Interfaces.General.Addable;
import LabMath.Interfaces.General.Divisible;
import LabMath.Interfaces.General.Multipliable;
import LabMath.Interfaces.General.Subtractable;
import LabMath.Interfaces.General.DoubleAddable;
import LabMath.Interfaces.General.DoubleDivisible;
import LabMath.Interfaces.General.DoubleMultipliable;
import LabMath.Interfaces.General.DoubleSubtractable;

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
