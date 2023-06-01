package LabMath.Matrixes;

import LabMath.Enums.Coords;
import LabMath.Interfaces.MathMatrix;
import jdk.jshell.spi.ExecutionControl;

import java.util.Random;

public class Matrix2D implements MathMatrix<Matrix2D> {
    private static final String ERROR_MULTIPLICATION = "Rows and columns are not equal";
    private final int rows;
    private final int cols;
    private final GeneralMatrix mat;

    public static void main(String[] args) {}

    @Override
    public String toString() {
        return mat.toString();
    }

    public Matrix2D(int rows, int cols) {
        mat = new GeneralMatrix(rows, cols);
        this.rows = rows;
        this.cols = cols;
    }

    public int getRows() {
        return this.rows;
    }

    public int getCols() {
        return this.cols;
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
    public Matrix2D getMul(Matrix2D other) {
        var cols = getCols();

        assert cols == other.getRows() : ERROR_MULTIPLICATION;

        var result = new Matrix2D(rows, cols);

        for(var i = 0; i < getRows(); ++i) {
           for(var j = 0; j < other.getCols(); ++j) {
               var value = 0;
               for(var k = 0; k < cols; ++k) {
                   value += this.mat.getAt(i, k) * other.mat.getAt(k, j);
               }
               result.setAt(value, i, j);
           }
        }

        return result;
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
        return this.mat.getDimensions();
    }

    @Override
    public double getAt(int... indexes) {
        return this.mat.getAt(indexes);
    }

    @Override
    public void setAt(double value, int... indexes) {
        this.mat.setAt(value, indexes);
    }

    @Override
    public int calcIndex(int... indexes) {
        return this.mat.calcIndex(indexes);
    }
}
