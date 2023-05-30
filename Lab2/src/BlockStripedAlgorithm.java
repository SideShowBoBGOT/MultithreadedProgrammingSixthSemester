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
        var rows = first.getRows();
        var cols = second.getCols();
        var otherCols = first.getCols();

        var result = new GeneralMatrix(rows, cols);

        var threads = new Thread[rows];

        for(var row = 0; row < rows; ++row) {
            var finalRow = row;
            threads[row] = new Thread(() -> {
                for(var col = 0; col < otherCols; ++col) {
                    var sum = 0.0;
                    for(var i = 0; i < cols; ++i) {
                        sum += first.getAt(finalRow, i) * second.getAt(i, col);
                    }
                    result.setAt(finalRow, col, sum);
                }
            });
            threads[row].start();
        }

        for(var thread : threads) {
            try {
                thread.join();
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
        }

        return result;
    }
}
