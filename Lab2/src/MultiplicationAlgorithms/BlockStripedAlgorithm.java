package MultiplicationAlgorithms;

import LabMath.Matrixes.Matrix2D;
import LabMath.Matrixes.Matrix2DFactory;

public class BlockStripedAlgorithm extends GeneralAlgorithm {

    BlockStripedAlgorithm(int threadsNum, Matrix2D first, Matrix2D second) {
        super(threadsNum, first, second);
    }

    public static void main(String[] args) {
        var matrixFactory = new Matrix2DFactory();
        var rows = 10;
        var cols = 10;
        var minVal = 0;
        var maxVal = 10;
        var threadsNum = 5;
        var first = matrixFactory.getRandom(rows, cols, minVal, maxVal);
        var second = matrixFactory.getRandom(rows, cols, minVal, maxVal);
        var algorithm = new BlockStripedAlgorithm(threadsNum, first, second);
        var result = algorithm.solve();
        System.out.println("First:\t" + first);
        System.out.println("Second:\t" + second);
        System.out.println("Result:\t" + result);
    }

    public Matrix2D solve() {
        var firstRows = first.getRows();
        var firstCols = first.getCols();
        var secondRows = second.getRows();
        var secondCols = second.getCols();

        assert firstCols == secondRows : ERROR_MULTIPLICATION;

        var result = new Matrix2D(firstRows, secondCols);
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

        return result;
    }
}
