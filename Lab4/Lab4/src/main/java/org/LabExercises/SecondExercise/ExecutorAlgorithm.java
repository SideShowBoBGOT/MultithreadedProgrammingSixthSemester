package org.LabExercises.SecondExercise;

import org.LabExercises.SecondExercise.Lab2MultiplicationAlgorithms.BlockStripedAlgorithm;
import org.LabMath.Matrixes.Matrix2D;
import org.LabMath.Matrixes.Matrix2DFactory;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

public class ExecutorAlgorithm {
	private static final String ERROR_MULTIPLICATION = "Rows and columns are not equal";
	private static final String ERROR_NUM_OF_THREADS = "Number of threads must be positive";
	private Matrix2D first;
	private Matrix2D second;
	private final ThreadPoolExecutor executor;

	public ExecutorAlgorithm(int threadsNum, Matrix2D first, Matrix2D second) {
		executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(threadsNum);
	}

	public static void main(String[] args) {
		var matrixFactory = new Matrix2DFactory();
		var rows = 10;
		var cols = 10;
		var minVal = 0;
		var maxVal = 10;
		var threadsNum = 5;
		var first = matrixFactory.getRandom(rows, cols, minVal, maxVal);
		var second = matrixFactory.getRandom(rows, cols, minVal, maxVal);
		var algorithm = new BlockStripedAlgorithm(threadsNum, first, second);
		var executorAlg = new ExecutorAlgorithm(threadsNum, first, second);
		var result = algorithm.solve();
		var resExecutor = executorAlg.solve();
		System.out.println("First:\t" + first);
		System.out.println("Second:\t" + second);
		System.out.println("Block:\t" + result);
		System.out.println("Executor:\t" + result);
	}

	public Matrix2D solve() {
		var poolSize = executor.getMaximumPoolSize();
		var endController = new CountDownLatch(poolSize);

		var firstRows = first.getRows();
		var firstCols = first.getCols();
		var secondRows = second.getRows();
		var secondCols = second.getCols();

		if(firstCols != secondRows) {
			throw new IllegalArgumentException(ERROR_MULTIPLICATION);
		}

		var result = new Matrix2D(firstRows, secondCols);
		var isRowsLess = firstRows < poolSize;
		var totalThreads = isRowsLess ? firstRows : poolSize;
		var step = isRowsLess ? 1 : poolSize;

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

		return result;
	}

}
