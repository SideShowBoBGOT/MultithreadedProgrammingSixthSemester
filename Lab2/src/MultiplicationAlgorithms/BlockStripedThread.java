package MultiplicationAlgorithms;

import LabMath.Matrixes.Matrix2D;

public class BlockStripedThread extends GeneralAlgorithmThread {
    private final int step;
    private final int firstRow;

    public BlockStripedThread(int firstRow, int step, Matrix2D first, Matrix2D second, Matrix2D result) {
        super(first, second, result);
        this.firstRow = firstRow;
        this.step = step;
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
