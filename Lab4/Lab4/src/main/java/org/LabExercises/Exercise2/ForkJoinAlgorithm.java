package org.LabExercises.Exercise2;

import org.LabMath.Matrixes.Matrix2D;
import org.LabMath.Matrixes.Matrix2DFactory;

import java.io.FileWriter;
import java.io.IOException;
import java.util.concurrent.CountDownLatch;
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
		if(first.getCols() != second.getRows()) {
			throw new IllegalArgumentException(ERROR_MULTIPLICATION);
		}

		var result = new Matrix2D(first.getRows(), second.getCols());
		var task = new ForkJointAlgorithmTask(0, first, second, result);
		pool.invoke(task);

		return result;

	}
}
