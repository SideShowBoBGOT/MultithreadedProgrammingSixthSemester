package org.LabExercises.SecondExercise;

import org.LabMath.Matrixes.Matrix2D;

import java.util.concurrent.ForkJoinPool;

public class ForkJoinAlgorithm implements MatrixAlgorithmInterface {
	private static final String ERROR_MULTIPLICATION = "Rows and columns are not equal";
	private final ForkJoinPool pool = ForkJoinPool.commonPool();
	private final Matrix2D first;
	private final Matrix2D second;

	public ForkJoinAlgorithm(Matrix2D first, Matrix2D second) {
		this.first = first;
		this.second = second;
	}

	@Override
	public Matrix2D solve() {
		var firstRows = first.getRows();
		var firstCols = first.getCols();
		var secondRows = second.getRows();
		var secondCols = second.getCols();

		if(firstCols != secondRows) {
			throw new IllegalArgumentException(ERROR_MULTIPLICATION);
		}

	}
}
