package org.MultiplicationAlgorithms.Fox;



import org.LabMath.Matrixes.Matrix2D;

import java.util.concurrent.Callable;

public class FoxAlgorithmTask implements Callable<Matrix2D> {
	private final Matrix2D first;
	private final Matrix2D second;
	private final Matrix2D result;

	public FoxAlgorithmTask(Matrix2D first, Matrix2D second, Matrix2D result){
		this.first = first;
		this.second = second;
		this.result = result;
	}

	@Override
	public Matrix2D call(){
		result.add(first.getMul(second));
		return result;
	}
}