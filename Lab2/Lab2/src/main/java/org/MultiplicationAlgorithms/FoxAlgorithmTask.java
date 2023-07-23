package org.MultiplicationAlgorithms;



import org.LabMath.Matrixes.Matrix2D;

import java.util.concurrent.Callable;

public class TaskFoxAlgorithm implements Callable<Matrix2D> {
	private Matrix2D first;
	private Matrix2D second;
	private Matrix2D result;

	public TaskFoxAlgorithm(Matrix2D first, Matrix2D second, Matrix2D result){
		this.first = first;
		this.second = second;
		this.result = result;
	}

	@Override
	public Matrix2D call(){

		return result.add(first.getMul(second));
	}
}