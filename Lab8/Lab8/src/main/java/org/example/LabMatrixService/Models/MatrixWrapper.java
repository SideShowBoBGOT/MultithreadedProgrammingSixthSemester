package org.example.LabMatrixService.Models;

import org.example.LabMath.Matrixes.GeneralMatrix;
import org.example.LabMath.Matrixes.Matrix2D;

public class MatrixWrapper {
	private final Matrix2D mat;
	public MatrixWrapper(int rows, int cols, double[] values) {
		mat = new Matrix2D(rows, cols);
		for(var i = 0; i < rows; ++i) {
			for(var j = 0; j < cols; ++j) {
				mat.setAt(values[i * cols + j], i, j);
			}
		}
	}
	public Matrix2D getMat() {
		return mat;
	}
}
