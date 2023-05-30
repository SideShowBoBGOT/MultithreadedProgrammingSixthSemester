package LabMath.Matrixes;

import LabMath.Interfaces.MathMatrix;
import LabMath.Vectors.GeneralVector;
import jdk.jshell.spi.ExecutionControl;

import java.util.Arrays;

public class GeneralMatrix implements MathMatrix<GeneralMatrix> {
    private static final String ERROR_INDEXES = "Indexes are less than 0";
    private static final String ERROR_DIMENSIONS = "Matrix dimensions not equal";
    private final int[] dimensions;
    private final int total;
    private final GeneralVector mat;

    public GeneralMatrix(int... dimensions) {
        this.dimensions = dimensions.clone();
        this.total = Arrays.stream(dimensions).sum();
        this.mat = new GeneralVector(this.total);
    }

    @Override
    public void add(GeneralMatrix other) {
        assert Arrays.equals(this.dimensions, other.dimensions) : ERROR_DIMENSIONS;
        for(var i = 0; i < total; ++i) {
            this.mat.setAt(i, other.mat.getAt(i));
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
    public void mul(GeneralMatrix other) throws ExecutionControl.NotImplementedException {
        throw new ExecutionControl.NotImplementedException("");
    }

    @Override
    public void set(GeneralMatrix other) {
        assert Arrays.equals(this.dimensions, other.dimensions) : ERROR_DIMENSIONS;
        for(var i = 0; i < this.total; ++i) {
            this.mat.setAt(i, this.mat.getAt(i));
        }
    }

    @Override
    public void sub(GeneralMatrix other) {
        assert Arrays.equals(this.dimensions, other.dimensions) : ERROR_DIMENSIONS;
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
        assert Arrays.stream(indexes).allMatch(e -> e >= 0) : ERROR_INDEXES;
        var index = Arrays.stream(indexes).sum();
        return this.mat.getAt(index);
    }

    @Override
    public void setAt(double value, int... indexes) {
        assert Arrays.stream(indexes).allMatch(e -> e >= 0) : ERROR_INDEXES;
        var index = Arrays.stream(indexes).sum();
        this.mat.setAt(index, value);
    }

    @Override
    public void div(GeneralMatrix other) throws ExecutionControl.NotImplementedException {
        throw new ExecutionControl.NotImplementedException("");
    }
}
