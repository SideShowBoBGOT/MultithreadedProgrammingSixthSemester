package org.example.LabMatrixService.Models;

import org.example.LabMath.Matrixes.Matrix2D;

public record MultiplyRequest(AlgType algType, Matrix2D first, Matrix2D second, int threadsNum) {
}
