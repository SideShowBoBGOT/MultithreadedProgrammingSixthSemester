package org.LabMath.Vectors;

import org.LabMath.Interfaces.MathVector;

import java.util.Arrays;

public class GeneralVector implements MathVector<GeneralVector> {
    private static final String ERROR_LENGTHS_NOT_EQUAL = "Lengths of points arguments are not equal";
    private double[] arguments;

    public GeneralVector(int length) {
        setLength(length);
    }

    public GeneralVector(GeneralVector other) {
        setLength(other.getLength());
        set(other);
    }

    public int getLength() {
        if(arguments == null) {
            return 0;
        }
        return arguments.length;
    }

    public void setLength(int length) {
        var currentLength = getLength();

        if(currentLength==length) return;

        var minLength = Math.min(currentLength, length);
        var args = new double[length];
        for(var i = 0; i < minLength; ++i) {
            args[i] = getAt(i);
        }

        arguments = args;
    }

    @Override
    public void set(GeneralVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, other.getAt(i));
        }
    }

    @Override
    public GeneralVector clone() {
        return new GeneralVector(this);
    }

    @Override
    public String toString() {
        return Arrays.toString(arguments);
    }

    private void checkSizesEqual(GeneralVector other) {
        assert getLength() == other.getLength() : ERROR_LENGTHS_NOT_EQUAL;
    }

    @Override
    public void add(GeneralVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) + other.getAt(i));
        }
    }

    @Override
    public void add(double value) {
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) + value);
        }
    }

    @Override
    public void sub(GeneralVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) - other.getAt(i));
        }
    }

    @Override
    public void sub(double value) {
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) - value);
        }
    }

    @Override
    public void mul(GeneralVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) * other.getAt(i));
        }
    }

    @Override
    public void mul(double value) {
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) * value);
        }
    }

    @Override
    public void div(GeneralVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) / other.getAt(i));
        }
    }

    @Override
    public void div(double value) {
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) / value);
        }
    }

    @Override
    public double getSize() {
        return Math.sqrt(getSizeSquared());
    }

    @Override
    public double getSizeSquared() {
        double s = 0;
        for(var i = 0; i < getLength(); ++i) {
            s += Math.pow(getAt(i), 2);
        }
        return s;
    }

    @Override
    public double getDotProduct(GeneralVector other) {
        checkSizesEqual(other);
        var prod = 0;
        for(var i = 0; i < getLength(); ++i) {
            prod += getAt(i) * other.getAt(i);
        }
        return prod;
    }

    @Override
    public double getDistance(GeneralVector other) {
        checkSizesEqual(other);
        var dist = 0.0;
        for(var i = 0; i < getLength(); ++i) {
            dist += Math.pow(getAt(i) - other.getAt(i), 2);
        }
        dist = Math.sqrt(dist);
        return dist;
    }

    @Override
    public GeneralVector getForwardVector() {
        var forwardVec = clone();
        var size = getSize();
        for(var i = 0; i < getLength(); ++i) {
            forwardVec.setAt(i, getAt(i) / size);
        }
        return forwardVec;
    }

    @Override
    public double getAt(int index) {
        return arguments[index];
    }

    @Override
    public void setAt(int index, double value) {
        arguments[index] = value;
    }

    @Override
    public GeneralVector getOpposite() {
        var v = clone();
        v.toOpposite();
        return v;
    }

    @Override
    public void toOpposite() {
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, -getAt(i));
        }
    }

}
