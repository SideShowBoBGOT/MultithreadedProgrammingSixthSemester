package LabMaths;

public class ArgumentsVector implements MathVector<ArgumentsVector> {
    private static final String ERROR_LENGTHS_NOT_EQUAL = "Lengths of points arguments are not equal";
    private double[] arguments;

    ArgumentsVector() {}

    public void setArguments(double[] inArguments) {
        arguments = inArguments.clone();
    }

    public double[] getArguments() {
        return arguments.clone();
    }

    private void checkSizesEqual(ArgumentsVector other) {
        assert arguments.length == other.arguments.length : ERROR_LENGTHS_NOT_EQUAL;
    }

    @Override
    public void add(ArgumentsVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < arguments.length; ++i) {
            arguments[i] += other.getArguments()[i];
        }
    }

    @Override
    public void sub(ArgumentsVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < arguments.length; ++i) {
            arguments[i] -= other.getArguments()[i];
        }
    }

    @Override
    public void multiply(ArgumentsVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < arguments.length; ++i) {
            arguments[i] *= other.getArguments()[i];
        }
    }

    @Override
    public void divide(ArgumentsVector other) {
        checkSizesEqual(other);
        for(var i = 0; i < arguments.length; ++i) {
            arguments[i] /= other.getArguments()[i];
        }
    }

    @Override
    public double size() {
        return Math.sqrt(sizeSquared());
    }

    @Override
    public double sizeSquared() {
        double s = 0;
        for(double argument : arguments) {
            s += Math.pow(argument, 2);
        }
        return s;
    }

    @Override
    public double dotProduct(ArgumentsVector other) {
        checkSizesEqual(other);
        var prod = 0;
        for(var i = 0; i < arguments.length; ++i) {
            prod += arguments[i] * other.getArguments()[i];
        }
        return prod;
    }

    @Override
    public double distance(ArgumentsVector other) {
        checkSizesEqual(other);
        var dist = 0.0;
        for(var i = 0; i < arguments.length; ++i) {
            dist += Math.pow(arguments[i] - other.getArguments()[i], 2);
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
}
