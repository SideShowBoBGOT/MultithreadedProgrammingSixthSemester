package org.MultiplicationAlgorithms.BlockStriped;

import org.LabMath.Matrixes.Matrix2D;

import java.util.concurrent.Callable;

public class BlockStripedAlgorithmTask implements Callable<Double> {
	private final Matrix2D first;
	private final Matrix2D second;
	private final int row;
	private final int col;

	public BlockStripedAlgorithmTask(int row, int col, Matrix2D first, Matrix2D second) {
		this.first = first;
		this.second = second;
		this.row = row;
		this.col = col;
	}

	@Override
	public Double call(){
		var element = 0.0;
		for(var i = 0; i < first.getCols(); ++i) {
			element += first.getAt(row, i) * second.getAt(i, col);
		}
		return element;
	}

}
