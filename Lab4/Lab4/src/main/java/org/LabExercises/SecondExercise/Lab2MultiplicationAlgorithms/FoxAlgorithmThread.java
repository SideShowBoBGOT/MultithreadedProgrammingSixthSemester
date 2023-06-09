package org.LabExercises.SecondExercise.Lab2MultiplicationAlgorithms;

import org.LabMath.Matrixes.Matrix2D;

public class FoxAlgorithmThread extends GeneralAlgorithmThread {
    private final int start;
    private final int blockSize;

    public FoxAlgorithmThread(int index, int blockSize, Matrix2D first, Matrix2D second, Matrix2D result) {
        super(first, second, result);
        this.start = index * blockSize;
        this.blockSize = blockSize;
    }

    @Override
    public void run() {
        var curRow = start;
        var curCol = start;
        var rows = first.getRows();
        var steps = rows / blockSize;
        for(var i = 0; i < steps; ++i) {
            for(var j = 0; j < steps; ++j) {
                mulMatrices(curCol, curRow, j * blockSize);
            }
            curRow += blockSize;
            curRow %= rows;
            curCol += blockSize;
            curCol %= rows;
        }
    }

    public void mulMatrices(int firstCol, int secondRow, int secondCol) {
        for(var i = start; i < start + blockSize; ++i) {
            for(var j = secondCol; j < secondCol + blockSize; ++j) {
                var value = 0.0;
                for(var k = 0; k < blockSize; ++k) {
                    value += first.getAt(i, firstCol + k) * second.getAt(secondRow + k, j);
                }
                result.setAt(result.getAt(i, j) + value, i, j);
            }
        }
    }
}
