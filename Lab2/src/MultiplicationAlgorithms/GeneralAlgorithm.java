package MultiplicationAlgorithms;

import LabMath.Matrixes.Matrix2D;

public abstract class GeneralAlgorithm {
    protected static final String ERROR_MULTIPLICATION = "Rows and columns are not equal";
    protected Thread[] threads;
    protected Matrix2D first;
    protected Matrix2D second;

    GeneralAlgorithm(int threadsNum, Matrix2D first, Matrix2D second) {
        setThreadsNum(threadsNum);
        setFirst(first);
        setSecond(second);
    }

    public void setThreadsNum(int threadsNum) {
        assert threadsNum > 0;
        if(this.threads != null && this.threads.length == threadsNum) return;
        this.threads = new Thread[threadsNum];
    }

    public void setFirst(Matrix2D first) {
        this.first = first;
    }

    public void setSecond(Matrix2D second) {
        this.second = second;
    }

    public abstract Matrix2D solve();
}
