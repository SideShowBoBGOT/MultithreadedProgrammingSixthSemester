package org.LabMatrixService.Models;

import org.LabMath.Matrixes.Matrix2D;

public record ClientMultiplyRequest(AlgType algType, Matrix2D first, Matrix2D second, int threadsNum) {
}
