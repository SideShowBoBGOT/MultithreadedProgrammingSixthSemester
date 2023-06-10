package org.LabExercises.SecondExercise;

import org.LabMath.Matrixes.Matrix2D;
import org.LabMath.Matrixes.Matrix2DFactory;

import java.io.FileWriter;
import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

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

	public static void main(String[] args) throws IOException {
		var file = new FileWriter("MatrixMultiplicationExecutor.txt");
		var matrixFactory = new Matrix2DFactory();
		var sizes = new int[]{100, 200, 300, 400};
		var threadsNum = new int[]{2, 3, 4, 5};
		var minVal = 0;
		var maxVal = 10;
		for(var s : sizes) {
			file.write("Size: " + s + "\n");
			var mat = matrixFactory.getRandom(s, s, minVal, maxVal);
			var startTime = System.currentTimeMillis();
			mat.getMul(mat);
			var singleDuration = System.currentTimeMillis() - startTime;
			file.write("\tSingle duration: " + singleDuration + "\n");
			for(var n : threadsNum) {
				file.write("\tExecutor threads: " + n + "\n");
				var start = System.currentTimeMillis();
				var executorAlg = new ExecutorAlgorithm(n, mat, mat);
				executorAlg.solve();
				var d = System.currentTimeMillis() - start;
				file.write("\t\tExecutor duration: " + d + "\n");
				file.write("\t\tExecutor efficiency: " + (double) singleDuration / d + "\n");
			}
		}
		file.close();
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

		var poolSize = executor.getMaximumPoolSize();
		var result = new Matrix2D(firstRows, secondCols);
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

		return result;
	}

}
