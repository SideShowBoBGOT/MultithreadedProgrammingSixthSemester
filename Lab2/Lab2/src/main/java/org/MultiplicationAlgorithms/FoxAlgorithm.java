package org.MultiplicationAlgorithms;

import org.LabMath.Matrixes.Matrix2D;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class FoxAlgorithm {
	public FoxAlgorithm() {}

	public Matrix2D multiply(int countThread, Matrix2D first, Matrix2D second) {
		var sizeMatrix2DM = (int) Math.sqrt(countThread - 1) + 1;
		var complexFirst = MatrixToComplexMatrix(first, sizeMatrix2DM);
		var complexSecond = MatrixToComplexMatrix(second, sizeMatrix2DM);

		var sizeInternalM = complexFirst[0][0].getCols();
		Matrix2D[][] resultMatrix2DM = new Matrix2D[sizeMatrix2DM][sizeMatrix2DM];
		for (var i = 0; i < sizeMatrix2DM; i++) {
			for (var j = 0; j < sizeMatrix2DM; j++) {
				resultMatrix2DM[i][j] = new Matrix2D(sizeInternalM, sizeInternalM);
			}
		}

		ExecutorService executor = Executors.newFixedThreadPool(countThread);
		for (var k = 0; k < sizeMatrix2DM; k++) {
			ArrayList<Future<Matrix2D>> futures = new ArrayList<Future<Matrix2D>>();
			for (var i = 0; i < sizeMatrix2DM; i++) {
				for (var j = 0; j < sizeMatrix2DM; j++) {
					TaskFoxAlgorithm task = new TaskFoxAlgorithm(
						complexFirst[i][(i + k) % sizeMatrix2DM],
						complexSecond[(i + k) % sizeMatrix2DM][j],
						resultMatrix2DM[i][j]);
					futures.add(executor.submit(task));
				}
			}

			for (var i = 0; i < sizeMatrix2DM; i++) {
				for (var j = 0; j < sizeMatrix2DM; j++) {
					try {
						resultMatrix2DM[i][j] = futures.get(i * sizeMatrix2DM + j).get();
					} catch (Exception ignored) {
					}
				}
			}
		}
		executor.shutdown();

		return ComplexMatrixToMatrix(resultMatrix2DM, first.getRows(), second.getCols());
	}

	private Matrix2D[][] MatrixToComplexMatrix(Matrix2D matrix, int size){
		var complexMatrix = new Matrix2D[size][size];
		var rows = matrix.getRows();
		var cols = matrix.getCols();
		var inner = ((cols - 1) / size) + 1;

		for(var i = 0; i < size; ++i) {
			for(var j = 0; j < size; ++j) {
				complexMatrix[i][j] = new Matrix2D(inner, inner);
				for(var k = 0; k < inner; ++k) {
					for (var l = 0; l < inner; ++l) {
						if(i * inner + k >= rows || j * inner + l >= cols){
							complexMatrix[i][j].setAt(0, k, l);
						} else {
							var element = matrix.getAt(i * inner + k, j * inner + l);
							complexMatrix[i][j].setAt(element, k, l);
						}
					}
				}
			}
		}
		return complexMatrix;
	}

	private Matrix2D ComplexMatrixToMatrix(Matrix2D[][] complexMatrix, int rows, int cols) {
		var matrix = new Matrix2D(rows, cols);
		for(var i = 0; i < complexMatrix.length; ++i) {
			for(var j = 0; j < complexMatrix[i].length; ++j) {
				var localRows = complexMatrix[i][j].getRows();
				var localCols = complexMatrix[i][j].getCols();
				for (var k = 0; k < localRows; k++) {
					for (var l = 0; l < localCols; l++) {
						var rowIndex = i * localRows + k;
						var colIndex = j * localCols + l;
						if(rowIndex >= rows || colIndex >= cols) continue;
						var el = complexMatrix[i][j].getAt(k, l);
						matrix.setAt(el, rowIndex, colIndex);
					}
				}
			}
		}

		return matrix;
	}
}
