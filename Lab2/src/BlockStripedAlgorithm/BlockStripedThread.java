package BlockStripedAlgorithm;

import LabMath.Matrixes.Matrix2D;

public class BlockStripedThread extends Thread {
    private final int firstRow;
    private final int step;
    private final Matrix2D first;
    private final Matrix2D second;
    private final Matrix2D result;

    public BlockStripedThread(int firstRow, int step, Matrix2D first, Matrix2D second, Matrix2D result) {
        this.first = first;
        this.step = step;
        this.second = second;
        this.firstRow = firstRow;
        this.result = result;
    }

    @Override
    public void run() {
        var firstRows = first.getRows();
        var firstCols = first.getCols();
        var secondCols = second.getCols();
        var curRow = firstRow;
        while(curRow < firstRows) {
            for(var j = 0; j < secondCols; ++j) {
                var value = 0.0;
                for(var k = 0; k < firstCols; ++k) {
                    value += first.getAt(curRow, k) * second.getAt(k, j);
                }
                result.setAt(value, curRow, j);
            }
            curRow += step;
        }

    }

}
