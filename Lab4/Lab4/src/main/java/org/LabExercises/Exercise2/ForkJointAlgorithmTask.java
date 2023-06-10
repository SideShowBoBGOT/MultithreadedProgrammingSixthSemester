package org.LabExercises.Exercise2;

import org.LabMath.Matrixes.Matrix2D;

import java.util.concurrent.RecursiveAction;

public class ForkJointAlgorithmTask extends RecursiveAction {
	private final Matrix2D first;
	private final Matrix2D second;
	private final Matrix2D result;
	private final int row;

	public ForkJointAlgorithmTask(int row, Matrix2D first,
								  Matrix2D second, Matrix2D result) {
		this.first = first;
		this.second = second;
		this.result = result;
		this.row = row;
	}

	@Override
	public void compute() {
		if(row >= result.getRows()) {
			return;
		}

		var rhsTask = new ForkJointAlgorithmTask(row + 1, first, second, result);
		rhsTask.fork();

		var firstCols = first.getCols();

		for(var j = 0; j < second.getCols(); ++j) {
			var val = 0.0;
			for(var k = 0; k < firstCols; ++k) {
				val += first.getAt(row, k) * second.getAt(k, j);
			}
			result.setAt(val, row, j);
		}

		rhsTask.join();
	}
}
