package org.example.LabMatrixService.Models;

import org.example.LabMath.Matrixes.Matrix2D;

public record MultiplyRequest(AlgType algType, MatrixWrapper first, MatrixWrapper second, int threadsNum) {
}
