package org.example.Models;

import org.example.LabMath.Matrixes.Matrix2D;

public record MultiplyRequest(Matrix2D first, Matrix2D second, int threadsNum) {
}
