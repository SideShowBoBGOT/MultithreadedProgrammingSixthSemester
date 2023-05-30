package LabMath.Matrixes;

import LabMath.Interfaces.MathMatrix;
import jdk.jshell.spi.ExecutionControl;

public class Matrix2D implements MathMatrix<Matrix2D> {
    private final GeneralMatrix mat;

    public Matrix2D(int rows, int cols) {
        mat = new GeneralMatrix(rows, cols);
    }
    @Override
    public void add(Matrix2D other) {
        this.mat.add(other.mat);
    }

    @Override
    public void div(Matrix2D other) throws ExecutionControl.NotImplementedException {
        throw new ExecutionControl.NotImplementedException("");
    }

    @Override
    public void add(double value) {
        this.mat.add(value);
    }

    @Override
    public void div(double value) {
        this.mat.div(value);
    }

    @Override
    public void mul(double value) {
        this.mat.mul(value);
    }

    @Override
    public void sub(double value) {
        this.mat.sub(value);
    }

    @Override
    public void mul(Matrix2D other) throws ExecutionControl.NotImplementedException {

    }

    @Override
    public void set(Matrix2D other) {
        this.mat.set(other.mat);
    }

    @Override
    public void sub(Matrix2D other) {
        this.mat.sub(other.mat);
    }

    @Override
    public int[] getDimensions() {
        return new int[0];
    }

    @Override
    public double getAt(int... indexes) {
        return this.mat.getAt(indexes);
    }

    @Override
    public void setAt(double value, int... indexes) {
        this.mat.setAt(value, indexes);
    }
}
