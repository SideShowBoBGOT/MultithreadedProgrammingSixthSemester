package org.example.LabMath.Interfaces;

import org.example.LabMath.Interfaces.General.*;

public interface MathMatrix<T> extends Cloneable, Addable<T>, Subtractable<T>, Divisible<T>,
        GetMultipliable<T>, Settable<T>, DoubleSubtractable, DoubleMultipliable, DoubleAddable, DoubleDivisible {
    int[] getDimensions();
    double getAt(int... indexes);
    void setAt(double value, int... indexes);
    int calcIndex(int... indexes);
}
