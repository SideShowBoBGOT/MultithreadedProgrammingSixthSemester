package org.example.LabMatrixService.Models;

import org.example.LabMath.Matrixes.Matrix2D;

public record RandomMultiplyRequest(AlgType algType, int rows, int cols, int seed, int threadsNum) {
}
