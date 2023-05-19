package LabMaths;

public class GenericPoint<T extends Number> implements MathOperate<GenericPoint<T>> {
    private static final String ERROR_LENGTHS_NOT_EQUAL = "Lengths of points arguments are not equal";
    private T[] arguments;

    GenericPoint(T[] inArguments) {
        arguments = inArguments.clone();
    }

    public T[] getArguments() {
        return arguments.clone();
    }

    private void checkSizesEqual(GenericPoint<T> other) {
        assert arguments.length == other.arguments.length : ERROR_LENGTHS_NOT_EQUAL;
    }

    @Override
    public void add(GenericPoint<T> other) {
        checkSizesEqual(other);
        for(var i = 0; i < arguments.length; ++i) {
            Double h = arguments[i].doubleValue() + other.getArguments()[i].doubleValue();
            arguments[i] = (T) (h);
        }
    }

    @Override
    public void sub(GenericPoint<T> other) {
        checkSizesEqual(other);

    }

    @Override
    public void multiply(GenericPoint<T> other) {
        checkSizesEqual(other);

    }

    @Override
    public void divide(GenericPoint<T> other) {
        checkSizesEqual(other);

    }

    public
}
