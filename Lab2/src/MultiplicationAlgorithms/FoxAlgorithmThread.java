package MultiplicationAlgorithms;

import LabMath.Matrixes.Matrix2D;

public class FoxAlgorithmThread extends GeneralAlgorithmThread {
    private final int startRow;
    private final int startCol;
    private final int blockSize;

    public FoxAlgorithmThread(int startRow, int startCol, int blockSize, Matrix2D first, Matrix2D second, Matrix2D result) {
        super(first, second, result);
        this.startRow = startRow;
        this.startCol = startCol;
        this.blockSize = blockSize;
    }

    @Override
    public void run() {
        var curRow = this.startRow;
        var curCol = this.startCol;
        var rows = first.getRows();
        var steps = rows / blockSize;
        for(var i = 0; i < steps; ++i) {



            curCol -= blockSize;
            if(curCol == -blockSize) curCol += rows;
            curCol += blockSize;
            curCol %= rows;
        }
    }
}
