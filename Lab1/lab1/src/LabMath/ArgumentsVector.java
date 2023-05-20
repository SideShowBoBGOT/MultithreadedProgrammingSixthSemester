package LabMath;

import java.util.Arrays;

public class ArgumentsVector implements MathVector<ArgumentsVector> {
    private static final String ERROR_LENGTHS_NOT_EQUAL = "Lengths of points arguments are not equal";
    private double[] arguments;

    ArgumentsVector(int length) {
        setLength(length);
    }

    ArgumentsVector(ArgumentsVector other) {
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
    public void set(ArgumentsVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, other.getAt(i));
        }
    }

    public static void main(String[] args) {
        var rr = new double[22];
        System.out.println(Arrays.toString(rr));
    }

    @Override
    public ArgumentsVector clone() {
        return new ArgumentsVector(this);
    }

    @Override
    public String toString() {
        return Arrays.toString(arguments);
    }

    private void checkSizesEqual(ArgumentsVector other) {
        assert getLength() == other.getLength() : ERROR_LENGTHS_NOT_EQUAL;
    }

    @Override
    public void add(ArgumentsVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) + other.getAt(i));
        }
    }

    @Override
    public void sub(ArgumentsVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) - other.getAt(i));
        }
    }

    @Override
    public void multiply(ArgumentsVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) * other.getAt(i));
        }
    }

    @Override
    public void divide(ArgumentsVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < getLength(); ++i) {
            setAt(i, getAt(i) / other.getAt(i));
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
    public double getDotProduct(ArgumentsVector other) {
        checkSizesEqual(other);
        var prod = 0;
        for(var i = 0; i < getLength(); ++i) {
            prod += getAt(i) * other.getAt(i);
        }
        return prod;
    }

    @Override
    public double getDistance(ArgumentsVector other) {
        checkSizesEqual(other);
        var dist = 0.0;
        for(var i = 0; i < getLength(); ++i) {
            dist += Math.pow(getAt(i) - other.getAt(i), 2);
        }
        dist = Math.sqrt(dist);
        return dist;
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
    public ArgumentsVector getOpposite() {
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
