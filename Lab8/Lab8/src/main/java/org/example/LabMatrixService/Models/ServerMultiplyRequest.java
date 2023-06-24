package org.example.LabMatrixService.Models;

public record ServerMultiplyRequest(AlgType algType, int rows, int cols, int seed, int threadsNum) {
}
