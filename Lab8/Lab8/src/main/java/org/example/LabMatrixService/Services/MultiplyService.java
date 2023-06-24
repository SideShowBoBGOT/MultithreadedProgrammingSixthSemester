package org.example.LabMatrixService.Services;

import org.example.LabMatrixService.Models.AlgType;
import org.example.LabMatrixService.Models.Result;
import org.example.LabMath.Matrixes.Matrix2D;

import java.util.ArrayList;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class MultiplyService {
	private final Matrix2D first;
	private final Matrix2D second;


	public MultiplyService(Matrix2D first, Matrix2D second, int threadsNum) {
		this.first = first;
		this.second = second;
	}

	public Result solve(int threadsNumber) {
		var startTime = System.currentTimeMillis();


		var endTime = System.currentTimeMillis();
		var result = new Result(
			new Matrix(
				resultMatrix,
				this.firstMatrix.getRows(),
				this.secondMatrix.getColumns()),
			endTime - startTime);

		return result;
	}

	private static Matrix[][] initMatrix(int matrixLength, int subMatrixSize) {
		var matrix = new Matrix[matrixLength][matrixLength];
		for (var i = 0; i < matrixLength; i++) {
			for (var j = 0; j < matrixLength; j++) {
				matrix[i][j] = new Matrix(subMatrixSize, subMatrixSize);
			}
		}

		return matrix;
	}
}