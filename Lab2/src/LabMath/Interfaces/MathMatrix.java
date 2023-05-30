package LabMath.Interfaces;

import LabMath.Interfaces.General.*;

public interface MathMatrix<T> extends Cloneable, Addable<T>, Subtractable<T>, Divisible<T>,
        Multipliable<T>, Settable<T>, DoubleSubtractable, DoubleMultipliable, DoubleAddable, DoubleDivisible {
    int[] getDimensions();
    double getAt(int... indexes);
    void setAt(double value, int... indexes);
}
