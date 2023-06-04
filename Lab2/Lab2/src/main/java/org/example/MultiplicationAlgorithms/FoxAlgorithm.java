package org.example.MultiplicationAlgorithms;

import org.example.LabMath.Matrixes.Matrix2D;
import org.example.LabMath.Matrixes.Matrix2DFactory;

import java.util.ArrayList;

public class FoxAlgorithm extends GeneralAlgorithm {
    private static final String ERROR_PROCS_NUM = "Number of processes must be a square number";
    private static final String ERROR_SQUARE_MATRIX = "Rows and columns are not equal";
    private static final String ERROR_BLOCK_SIZE = "Matrix size must be divisible by number of threads";
    
    FoxAlgorithm(int threadsNum, Matrix2D first, Matrix2D second) {
        super(threadsNum, first, second);
    }

    public static void main(String[] args) {
        var matrixFactory = new Matrix2DFactory();
        var rows = 18;
        var cols = 18;
        var minVal = 0;
        var maxVal = 10;
        var threadsNum = 9;

        var first = matrixFactory.getRandom(rows, cols, minVal, maxVal);
        var second = matrixFactory.getRandom(rows, cols, minVal, maxVal);

        var algorithm = new FoxAlgorithm(threadsNum, first, second);
        var striped = new BlockStripedAlgorithm(threadsNum, first, second);
        var result = algorithm.solve();
        var stripedResult = striped.solve();
        System.out.println("First:\t" + first);
        System.out.println("Second:\t" + second);
        System.out.println("Result:\t" + result);
        System.out.println("Striped:\t" + stripedResult);
    }

    @Override
    public void setThreadsNum(int threadsNum) {
        var gridSize = (int) Math.sqrt(threadsNum);
        if(gridSize * gridSize != threadsNum) {
            throw new IllegalArgumentException(ERROR_PROCS_NUM);
        }
        super.setThreadsNum(threadsNum);
    }

    private void checkIfSquare(Matrix2D matrix) {
        if(!matrix.isSquare()) {
            throw new IllegalArgumentException(ERROR_SQUARE_MATRIX);
        }
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
        var rows = first.getRows();

        var blockSize = rows / threads.length;

        if(rows % threads.length != 0) {
            throw new IllegalArgumentException(ERROR_BLOCK_SIZE);
        }

        var matrices = new Matrix2D[threads.length];
        for(var i = 0; i < threads.length; ++i) {
            matrices[i] = new Matrix2D(rows, rows);
            threads[i] = new FoxAlgorithmThread(i, blockSize, first, second, matrices[i]);
            threads[i].start();
        }
        
        for(var t : threads) {
            try {
                t.join();
            } catch(InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        if(threads.length == 1) {
            return matrices[0];
        }

        for(var i = 1; i < matrices.length; ++i) {
            matrices[0].add(matrices[i]);
        }

        return matrices[0];
    }
}
