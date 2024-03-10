package org.LabMath.Interfaces;

import org.LabMath.Interfaces.General.*;

public interface MathMatrix<T> extends Cloneable, Addable<T>, Subtractable<T>, Divisible<T>, Comparable<T>,
        GetMultipliable<T>, Settable<T>, DoubleSubtractable, DoubleMultipliable, DoubleAddable, DoubleDivisible {
    int[] getDimensions();
    double getAt(int... indexes);
    void setAt(double value, int... indexes);
    int calcIndex(int... indexes);
}
