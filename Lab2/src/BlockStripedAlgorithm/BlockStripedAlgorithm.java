package BlockStripedAlgorithm;

import LabMath.Enums.Coords;
import LabMath.Matrixes.GeneralMatrix;
import LabMath.Matrixes.Matrix2D;

public class BlockStripedAlgorithm {
    private static final String ERROR_MULTIPLICATION = "Rows and columns are not equal";
    private Thread[] threads;
    private Matrix2D first;
    private Matrix2D second;
    private Matrix2D result;

    BlockStripedAlgorithm(int threadsNum, Matrix2D first, Matrix2D second) {
        setThreadsNum(threadsNum);
        setFirst(first);
        setSecond(second);
    }

    public void setThreadsNum(int threadsNum) {
        assert threadsNum > 0;
        if(this.threads.length == threadsNum) return;
        this.threads = new Thread[threadsNum];
    }

    public void setFirst(Matrix2D first) {
        this.first = first;
    }

    public void setSecond(Matrix2D second) {
        this.second = second;
    }

    public void solve() {
        var firstRows = first.getRows();
        var firstCols = first.getCols();
        var secondRows = second.getRows();
        var secondCols = second.getCols();

        assert firstCols == secondRows : ERROR_MULTIPLICATION;

        this.result = new Matrix2D(firstRows, secondCols);
        var isRowsLess = firstRows < threads.length;
        var totalThreads = isRowsLess ? firstRows : threads.length;
        var step = isRowsLess ? 1 : threads.length;

        for(var i = 0; i < totalThreads; ++i) {
            threads[i] = new BlockStripedThread(i, step, first, second, result);
        }

        for(var i = 0; i < totalThreads; ++i) {
            threads[i].start();
        }

        for(var i = 0; i < totalThreads; ++i) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
