package LabMath.Matrixes;

import LabMath.Interfaces.MathMatrix;
import LabMath.Vectors.GeneralVector;
import jdk.jshell.spi.ExecutionControl;

import java.util.Arrays;

public final class GeneralMatrix implements MathMatrix<GeneralMatrix> {
    private static final String ERROR_INDEXES = "Indexes are less than 0";
    private static final String ERROR_DIMENSIONS = "Matrix dimensions not equal";
    private static final String ERROR_DIMENSION_INDEXES = "Indexes length is not equal to amount of dimension";
    private final int[] dimensions;
    private final int total;
    private final GeneralVector mat;

    public GeneralMatrix(int... dimensions) {
        this.dimensions = dimensions.clone();
        var t = 1;
        for(var d : dimensions) t *= d;
        this.total = t;
        this.mat = new GeneralVector(this.total);
    }

    private String doDraw(int[] indexes, int dimension) {
        var res = new StringBuilder();
        res.append("{");
        for(var i = 0; i < this.dimensions[dimension]; ++i) {
            indexes[dimension] = i;
            if(dimension == this.dimensions.length - 1) {
                res.append(this.mat.getAt(calcIndex(indexes)));
            } else {
                res.append(doDraw(indexes, dimension + 1));
            }
            res.append(this.dimensions[dimension] - 1 == i ? "" : ", ");
        }
        res.append("}");
        return res.toString();
    }

    @Override
    public String toString() {
        var indexes = new int[this.dimensions.length];
        return doDraw(indexes, 0);
    }

    private void checkDimensions(int[] dimensions) {
        if(!Arrays.equals(this.dimensions, dimensions)) {
            throw new IllegalArgumentException(ERROR_DIMENSIONS);
        }
    }

    private void checkIndexes(int[] indexes) {
        if(!Arrays.stream(indexes).allMatch(e -> e >= 0)) {
            throw new IllegalArgumentException(ERROR_INDEXES);
        }
    }
    
    @Override
    public void add(GeneralMatrix other) {
        checkDimensions(other.dimensions);
        for(var i = 0; i < total; ++i) {
            this.mat.setAt(i, this.mat.getAt(i) + other.mat.getAt(i));
        }
    }

    @Override
    public void add(double value) {
        for(var i = 0; i < this.total; ++i) {
            this.mat.setAt(i, this.mat.getAt(i) + value);
        }
    }

    @Override
    public void div(double value) {
        for(var i = 0; i < this.total; ++i) {
            this.mat.setAt(i, this.mat.getAt(i) / value);
        }
    }

    @Override
    public void mul(double value) {
        for(var i = 0; i < this.total; ++i) {
            this.mat.setAt(i, this.mat.getAt(i) * value);
        }
    }

    @Override
    public void sub(double value) {
        for(var i = 0; i < this.total; ++i) {
            this.mat.setAt(i, this.mat.getAt(i) - value);
        }
    }

    @Override
    public GeneralMatrix getMul(GeneralMatrix other) throws ExecutionControl.NotImplementedException {
        throw new ExecutionControl.NotImplementedException("");
    }

    @Override
    public void set(GeneralMatrix other) {
        checkDimensions(other.dimensions);
        for(var i = 0; i < this.total; ++i) {
            this.mat.setAt(i, this.mat.getAt(i));
        }
    }

    @Override
    public void sub(GeneralMatrix other) {
        checkDimensions(other.dimensions);
        for(var i = 0; i < total; ++i) {
            this.mat.setAt(i, other.mat.getAt(i) - other.mat.getAt(i));
        }
    }

    @Override
    public int[] getDimensions() {
        return dimensions.clone();
    }

    @Override
    public double getAt(int... indexes) {
        checkIndexes(indexes);
        return this.mat.getAt(this.calcIndex(indexes));
    }

    @Override
    public void setAt(double value, int... indexes) {
        checkIndexes(indexes);
        var index = this.calcIndex(indexes);
        this.mat.setAt(index, value);
    }

    @Override
    public void div(GeneralMatrix other) throws ExecutionControl.NotImplementedException {
        throw new ExecutionControl.NotImplementedException("");
    }

    @Override
    public int calcIndex(int... indexes) {
        if(indexes.length != dimensions.length) {
            throw new IllegalArgumentException(ERROR_DIMENSION_INDEXES);
        }
        var index = 0;
        var mult = 1;
        for(var i : dimensions) mult *= i;
        for(var i = 0; i < indexes.length; ++i) {
            mult /= dimensions[i];
            index += indexes[i] * mult;
        }
        return index;
    }
}
