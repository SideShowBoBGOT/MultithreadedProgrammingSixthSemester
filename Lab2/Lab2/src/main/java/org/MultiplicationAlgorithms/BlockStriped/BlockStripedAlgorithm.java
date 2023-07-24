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

		for(var i = 0; i < secondCols; ++i) {
			//System.out.println("Iteration: " + i);
			for(var row = 0; row < firstRows; ++row) {
				var col = row - i;
				col = col < 0 ? col + secondCols : col;
				//System.out.println("\tProc: " + row + "\tCol: " + col);
				var task = new BlockStripedAlgorithmTask(row, col, first, second);
				callables.add(task);
			}
			try {
				futures.addAll(executor.invokeAll(callables));
				callables.clear();
			} catch (InterruptedException e) {
				throw new RuntimeException(e);
			}

		}
		executor.shutdown();
		try {
			for(var i = 0; i < secondCols; ++i) {
				for(var row = 0; row < firstRows; ++row) {
					var col = row - i;
					col = col < 0 ? col + secondCols : col;
					var future = futures.get(i * firstRows + row);
					result.setAt(future.get(), row, col);
				}
			}
		} catch (InterruptedException | ExecutionException e) {
			throw new RuntimeException(e);
		}

		return result;
	}
}