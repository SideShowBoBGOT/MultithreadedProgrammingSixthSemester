package org.MultiplicationAlgorithms;

import org.LabMath.Matrixes.Matrix2D;

import java.util.concurrent.Callable;

public class BlockStripedAlgorithmTask implements Callable<Double> {
	private final Matrix2D first;
	private final Matrix2D second;
	private final Integer index;

	public BlockStripedAlgorithmTask(int firstRow, Matrix2D first, Matrix2D second) {
		this.first = first;
		this.second = second;
		this.index = firstRow;
	}

	@Override
	public Double call(){
		var element = 0.0;
		for(int j = 0; j < first.getCols(); j++) {
			element += first.getAt(index, j) * second.getAt(j, index);
		}
		return element;
	}

}
