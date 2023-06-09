package org.LabExercises.SecondExercise.Lab2MultiplicationAlgorithms;

import org.LabMath.Matrixes.Matrix2D;

public abstract class GeneralAlgorithm {
    protected static final String ERROR_MULTIPLICATION = "Rows and columns are not equal";
    protected static final String ERROR_NUM_OF_THREADS = "Number of threads must be positive";

    protected Thread[] threads;
    protected Matrix2D first;
    protected Matrix2D second;

    public GeneralAlgorithm() {}

    protected GeneralAlgorithm(int threadsNum, Matrix2D first, Matrix2D second) {
        setThreadsNum(threadsNum);
        setFirst(first);
        setSecond(second);
    }

    public void setThreadsNum(int threadsNum) {
        if(threadsNum <= 0) {
            throw new IllegalArgumentException(ERROR_NUM_OF_THREADS);
        }
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
