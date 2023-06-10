package org.LabExercises.Exercise2;

import org.LabMath.Matrixes.Matrix2D;
import org.LabMath.Matrixes.Matrix2DFactory;

import java.io.FileWriter;
import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.function.IntSupplier;

public class ExecutorAlgorithm implements MatrixAlgorithmInterface {
	private static final String ERROR_MULTIPLICATION = "Rows and columns are not equal";
	private final Matrix2D first;
	private final Matrix2D second;
	private final ThreadPoolExecutor executor;

	public ExecutorAlgorithm(int threadsNum, Matrix2D first, Matrix2D second) {
		this.first = first;
		this.second = second;
		executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(threadsNum);
	}

	@Override
	public Matrix2D solve() {
		if(first.getCols() != second.getRows()) {
			throw new IllegalArgumentException(ERROR_MULTIPLICATION);
		}

		var firstRows = first.getRows();
		var poolSize = executor.getMaximumPoolSize();
		var result = new Matrix2D(firstRows, second.getCols());
		var isRowsLess = firstRows < poolSize;
		var totalThreads = isRowsLess ? firstRows : poolSize;
		var step = isRowsLess ? 1 : poolSize;
		var endController = new CountDownLatch(totalThreads);

		for(var i = 0; i < totalThreads; ++i) {
			var task = new ExecutorAlgorithmTask(step, i, first,
				second, result, endController);
			executor.execute(task);
		}

		try {
			endController.await();
		} catch(InterruptedException ex) {
			ex.printStackTrace();
		}

		executor.shutdown();

		return result;
	}

}
