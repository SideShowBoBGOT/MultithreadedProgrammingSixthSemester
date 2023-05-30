import LabMath.Matrixes.GeneralMatrix;

public class BlockStripedAlgorithm {
    private static final String ERROR_MESSAGE = "Cols of first matrix must be equal to rows of second matrix";
    private final GeneralMatrix first;
    private final GeneralMatrix second;

    BlockStripedAlgorithm(GeneralMatrix first, GeneralMatrix second) {
        assert first.getCols() == second.getRows() : ERROR_MESSAGE;
        this.first = first;
        this.second = second;
    }

    public GeneralMatrix solve() {
    }
}
