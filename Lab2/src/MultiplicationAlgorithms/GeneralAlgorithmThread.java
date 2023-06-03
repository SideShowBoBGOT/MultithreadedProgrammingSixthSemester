package MultiplicationAlgorithms;

import LabMath.Matrixes.Matrix2D;

public abstract class GeneralAlgorithmThread extends Thread {
    protected Matrix2D first;
    protected Matrix2D second;
    protected Matrix2D result;

    public GeneralAlgorithmThread(Matrix2D first, Matrix2D second, Matrix2D result) {
        this.first = first;
        this.second = second;
        this.result = result;
    }

    @Override
    public abstract void run();
}
