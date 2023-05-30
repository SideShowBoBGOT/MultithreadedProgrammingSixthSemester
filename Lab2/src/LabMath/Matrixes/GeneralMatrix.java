package LabMath.Matrixes;

import LabMath.Interfaces.MathMatrix;
import LabMath.Vectors.GeneralVector;

import java.util.Arrays;

public class GeneralMatrix implements MathMatrix<GeneralMatrix> {
    private final int[] dimensions;
    private final int total;
    private final GeneralVector mat;

    public GeneralMatrix(int[] dimensions) {
        this.dimensions = dimensions.clone();
        this.total = Arrays.stream(dimensions).sum();
        this.mat = new GeneralVector(this.total);
    }

    @Override
    public double[] getMat() {
        return this.mat;
    }

    @Override
    public void add(GeneralMatrix other) {
        for(var i = 0; i < total; ++i) {
            this.mat[i] += other.mat[i];
        }
    }

    @Override
    public void add(double value) {
        for(var i = 0; i < this.total; ++i) {
            this.mat[i] += value;
        }
    }

    @Override
    public void div(double other) {
        for(var i = 0; i < this.total; ++i) {
            this.mat[i] += other;
        }
    }

    @Override
    public void mul(double other) {

    }

    @Override
    public void sub(double other) {

    }

    @Override
    public void mul(GeneralMatrix other) {

    }

    @Override
    public void set(GeneralMatrix other) {

    }

    @Override
    public void sub(GeneralMatrix other) {

    }

    @Override
    public int[] getDimensions() {
        return new int[0];
    }

    @Override
    public double getAt(int[] indexes) {
        return 0;
    }

    @Override
    public void setAt(int[] indexes, double value) {

    }
}
