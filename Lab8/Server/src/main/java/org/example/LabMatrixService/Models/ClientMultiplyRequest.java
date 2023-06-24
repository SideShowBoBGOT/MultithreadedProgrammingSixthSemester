package org.example.LabMatrixService.Models;

public record ClientMultiplyRequest(AlgType algType, MatrixWrapper first, MatrixWrapper second, int threadsNum) {
}
