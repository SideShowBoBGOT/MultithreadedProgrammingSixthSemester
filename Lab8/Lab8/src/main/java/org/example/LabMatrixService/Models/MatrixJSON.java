package org.example.LabMatrixService.Models;

import com.fasterxml.jackson.annotation.JsonProperty;
import org.example.LabMath.Matrixes.Matrix2D;

public class MatrixJSON {
	private final Matrix2D mat;
	public MatrixJSON(@JsonProperty("rows") int rows,
					  @JsonProperty("cols") int cols) {
		this.mat = new Matrix2D(rows, cols);
	}
	Matrix2D getMat() {
		return mat;
	}
}
