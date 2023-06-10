package org.LabExercises.Exercise2;

import org.LabMath.Matrixes.Matrix2D;

import java.util.concurrent.RecursiveAction;
import java.util.stream.DoubleStream;
import java.util.stream.IntStream;

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
		IntStream.range(0, second.getCols()).forEach(j -> {
			var val = IntStream.range(0, firstCols).mapToDouble(k ->
				first.getAt(row, k) * second.getAt(k, j)
			).sum();
			result.setAt(val, row, j);
		});

		rhsTask.join();
	}
}
