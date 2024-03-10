package org.MultiplicationAlgorithms.Fox;

import org.LabMath.Matrixes.Matrix2D;
import org.MultiplicationAlgorithms.MultiplyAlgo;

import java.util.ArrayList;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class FoxAlgorithm implements MultiplyAlgo {
	public FoxAlgorithm() {}

	@Override
	public Matrix2D multiply(int countThread, Matrix2D first, Matrix2D second) {
		var complexSize = (int) Math.sqrt(countThread - 1) + 1;
		var complexFirst = MatrixToComplexMatrix(first, complexSize);
		var complexSecond = MatrixToComplexMatrix(second, complexSize);

		var innerSize = complexFirst[0][0].getCols();
		var complex = new Matrix2D[complexSize][complexSize];
		for(var i = 0; i < complexSize; ++i) {
			for(var j = 0; j < complexSize; ++j) {
				complex[i][j] = new Matrix2D(innerSize, innerSize);
			}
		}

		var executor = Executors.newFixedThreadPool(countThread);
		for(var k = 0; k < complexSize; ++k) {
			var futures = new ArrayList<Future<Matrix2D>>();
			for(var i = 0; i < complexSize; ++i) {
				for(var j = 0; j < complexSize; ++j) {
					var index = (i + k) % complexSize;
					var task = new FoxAlgorithmTask(
						complexFirst[i][index],
						complexSecond[index][j],
						complex[i][j]);
					futures.add(executor.submit(task));
				}
			}

			for(var i = 0; i < complexSize; ++i) {
				for(var j = 0; j < complexSize; ++j) {
					try {
						complex[i][j] = futures.get(i * complexSize + j).get();
					} catch (Exception ignored) {}
				}
			}
		}
		executor.shutdown();
		return ComplexMatrixToMatrix(complex, first.getRows(), second.getCols());
	}

	private Matrix2D[][] MatrixToComplexMatrix(Matrix2D matrix, int size){
		var complexMatrix = new Matrix2D[size][size];
		var rows = matrix.getRows();
		var cols = matrix.getCols();
		var inner = ((cols - 1) / size) + 1;
		for(var i = 0; i < size; ++i) {
			for(var j = 0; j < size; ++j) {
				complexMatrix[i][j] = new Matrix2D(inner, inner);
				var local = complexMatrix[i][j];
				for(var k = 0; k < inner; ++k) {
					for (var l = 0; l < inner; ++l) {
						var rowIndex = i * inner + k;
						var colIndex = j * inner + l;
						if(rowIndex >= rows || colIndex >= cols){
							local.setAt(0, k, l);
							continue;
						}
						var element = matrix.getAt(rowIndex, colIndex);
						local.setAt(element, k, l);
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
				var local = complexMatrix[i][j];
				var localRows = local.getRows();
				var localCols = local.getCols();
				for (var k = 0; k < localRows; ++k) {
					for (var l = 0; l < localCols; ++l) {
						var rowIndex = i * localRows + k;
						var colIndex = j * localCols + l;
						if(rowIndex >= rows || colIndex >= cols) continue;
						var el = local.getAt(k, l);
						matrix.setAt(el, rowIndex, colIndex);
					}
				}
			}
		}

		return matrix;
	}
}
