package org.MultiplicationAlgorithms.BlockStriped;

import org.LabMath.Matrixes.Matrix2D;
import org.MultiplicationAlgorithms.MultiplyAlgo;

import java.util.concurrent.Future;
import java.util.ArrayList;
import java.util.concurrent.*;

public class BlockStripedAlgorithm implements MultiplyAlgo {
	protected static final String ERROR_MULTIPLICATION = "Rows and columns are not equal";
	protected static final String ERROR_NUM_OF_THREADS = "Number of threads must be positive";
	
	public BlockStripedAlgorithm() {}

	@Override
	public Matrix2D multiply(int threadsNum, Matrix2D first, Matrix2D second) {
		if(threadsNum <= 0) {
			throw new IllegalArgumentException(ERROR_NUM_OF_THREADS);
		}

		var firstRows = first.getRows();
		var firstCols = first.getCols();
		var secondRows = second.getRows();
		var secondCols = second.getCols();

		if(firstCols != secondRows) {
			throw new IllegalArgumentException(ERROR_MULTIPLICATION);
		}

		var result = new Matrix2D(firstRows, secondCols);

		var executor = Executors.newFixedThreadPool(threadsNum);
		var callables = new ArrayList<BlockStripedAlgorithmTask>();
		var futures = new ArrayList<Future<Double>>();

		for(int i = 0; i < firstRows; ++i) {
			for(int j = 0; j < secondCols; ++j) {
				var index = (j + i) % firstRows;
				var task = new BlockStripedAlgorithmTask(index, first, second);
				callables.add(task);
			}

			try{
				futures.addAll(executor.invokeAll(callables));
				callables.clear();
			} catch (InterruptedException e) {
				throw new RuntimeException(e);
			}

		}
		executor.shutdown();

		try{
			for (int i = 0; i < firstRows; i++) {
				for (int j = 0; j < secondCols; j++) {
					var future = futures.get(i * secondCols + j);
					var index = (j + i) % firstRows;
					result.setAt(future.get(), index, j);
				}
			}
		}
		catch (InterruptedException | ExecutionException e) {
			throw new RuntimeException(e);
		}

		return result;
	}
}