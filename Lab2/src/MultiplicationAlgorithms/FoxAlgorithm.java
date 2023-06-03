package MultiplicationAlgorithms;

import LabMath.Matrixes.Matrix2D;

public class FoxAlgorithm extends GeneralAlgorithm {
    private static final String ERROR_PROCS_NUM = "Number of processes must be a square number";
    private static final String ERROR_SQUARE_MATRIX = "Rows and columns are not equal";

    FoxAlgorithm(int threadsNum, Matrix2D first, Matrix2D second) {
        super(threadsNum, first, second);
    }

    @Override
    public void setThreadsNum(int threadsNum) {
        var gridSize = (int) Math.sqrt(threadsNum);
        assert gridSize * gridSize == threadsNum : ERROR_PROCS_NUM;
        super.setThreadsNum(threadsNum);
    }

    private void checkIfSquare(Matrix2D matrix) {
        assert matrix.isSquare() : ERROR_SQUARE_MATRIX;
    }

    @Override
    public void setFirst(Matrix2D first) {
        checkIfSquare(first);
        super.setFirst(first);
    }

    @Override
    public void setSecond(Matrix2D second) {
        checkIfSquare(second);
        super.setSecond(second);
    }

    @Override
    public Matrix2D solve() {
        var totalThreads = Math.min(threads.length, first.getRows());
        setThreadsNum(totalThreads);

        var blockSize = first.getRows() / totalThreads;
        for(var i = 0; i < threads.length; ++i) {

        }
    }
}
