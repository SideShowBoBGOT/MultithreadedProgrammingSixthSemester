package org.LabExercises.Exercise2;

import org.LabMath.Matrixes.Matrix2D;

import java.util.concurrent.CountDownLatch;

public class ExecutorAlgorithmTask implements Runnable {
	private final Matrix2D first;
	private final Matrix2D second;
	private final Matrix2D result;
	private final int step;
	private final int firstRow;
	private final CountDownLatch endController;

	public ExecutorAlgorithmTask(int step, int firstRow, Matrix2D first, Matrix2D second,
								 Matrix2D result, CountDownLatch endController) {
		this.first = first;
		this.second = second;
		this.result = result;
		this.step = step;
		this.firstRow = firstRow;
		this.endController = endController;
	}

	@Override
	public void run() {
		var firstRows = first.getRows();
		var firstCols = first.getCols();
		var secondCols = second.getCols();
		var curRow = firstRow;
		while(curRow < firstRows) {
			for(var j = 0; j < secondCols; ++j) {
				var value = 0.0;
				for(var k = 0; k < firstCols; ++k) {
					var lhs = first.getAt(curRow, k);
					var rhs = second.getAt(k, j);
					value +=  lhs * rhs;
				}
				result.setAt(value, curRow, j);
			}
			curRow += step;
		}
		endController.countDown();
	}
}