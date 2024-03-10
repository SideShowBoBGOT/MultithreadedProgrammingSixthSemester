package org.LabMatrixService.Services;


import org.LabMath.Matrixes.Matrix2D;
import org.LabMatrixService.Models.AlgType;
import org.MultiplicationAlgorithms.BlockStriped.BlockStripedAlgorithm;
import org.MultiplicationAlgorithms.Fox.FoxAlgorithm;

public class MultiplyService {
    public MultiplyService() {}

    public static Matrix2D solve(AlgType algType, int threadsNum, Matrix2D first, Matrix2D second) {
        return switch(algType) {
            case BLOCK_STRIPED -> new BlockStripedAlgorithm().multiply(threadsNum, first, second);
            case FOX -> new FoxAlgorithm().multiply(threadsNum, first, second);
            case SINGLE -> first.getMul(second);
        };
    }
}
