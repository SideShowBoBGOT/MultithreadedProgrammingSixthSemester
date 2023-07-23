package test;

import com.github.sh0nk.matplotlib4j.*;
import org.LabMath.Matrixes.Matrix2DFactory;
import org.MultiplicationAlgorithms.BlockStriped.BlockStripedAlgorithm;
import org.MultiplicationAlgorithms.Fox.FoxAlgorithm;
import org.MultiplicationAlgorithms.MultiplyAlgo;

import java.io.*;
import java.util.ArrayList;

public class MatrixTester {
    private static final int minVal = -10;
    private static final int maxVal = 10;
    private static final String FILE_NAME = "results.csv";
    private static final String DELIMETER = "\t";
    private static final String LEGEND_POSITION = "upper left";
    private static final String X_LABEL = "Matrix size";
    private static final String Y_LABEL = "Milliseconds";
    private static final String SPEEDUP_Y_LABEL = "Speedup";
    private static final String PLOT_FILE = "plot.png";
    private static final String SPEEDUP_PLOT_FILE = "speedup_plot.png";


    private static class AlgorithmResult {
        public final long milliseconds;
        public final int threadsNum;
        public final int size;
        public final String name;
        public final double speedup;

        public AlgorithmResult(long time, int threadsNum, int size, double speedup, String name) {
            this.milliseconds = time;
            this.threadsNum = threadsNum;
            this.size = size;
            this.name = name;
            this.speedup = speedup;
        }

        @Override
        public String toString() {
            return String.format("%s\t%d\t%d\t%d\t%f", name, threadsNum, size, milliseconds, speedup);
        }
    }

    public static void main(String[] args) throws PythonExecutionException, IOException {
            var tester = new MatrixTester();
            var threadsNums = new int[] {2, 4, 10};
            var matrixSizes = new int[] {100, 300, 500, 700, 1000, 1200, 1400, 1500, 1700, 1800, 2000};
            tester.testAlgorithm(threadsNums, matrixSizes);
            tester.plotStatistic();
            tester.plotSpeedup();
    }

    public void plotStatistic() throws IOException, PythonExecutionException {
        Plot plt = Plot.create();
        var results = readStatistic();
        var algorithms = results.stream().map(r -> r.name).distinct().toList();
        var threadNums = results.stream().map(r -> r.threadsNum).distinct().toList();
        for(var a : algorithms) {
            var filtered = results.stream().filter(r -> r.name.equals(a)).toList();
            for(var threadNum : threadNums) {
                var filteredByThreadNum = filtered.stream().filter(r -> r.threadsNum == threadNum).toList();
                var x = filteredByThreadNum.stream().map(r -> r.size).toList();
                var y = filteredByThreadNum.stream().map(r -> r.milliseconds).toList();
                plt.plot().add(x, y).label(a + " " + threadNum);
            }
        }
        plt.legend().loc(LEGEND_POSITION);
        plt.xlabel(X_LABEL);
        plt.ylabel(Y_LABEL);
        plt.savefig(PLOT_FILE);
        plt.show();
    }

    private void plotSpeedup() throws IOException, PythonExecutionException {
        Plot plt = Plot.create();
        var results = readStatistic();
        var algorithms = results.stream().map(r -> r.name).distinct().toList();
        var threadNums = results.stream().map(r -> r.threadsNum).distinct().toList();
        for(var a : algorithms) {
            var filtered = results.stream().filter(r -> r.name.equals(a)).toList();
            for(var threadNum : threadNums) {
                var filteredByThreadNum = filtered.stream().filter(r -> r.threadsNum == threadNum).toList();
                var x = filteredByThreadNum.stream().map(r -> r.size).toList();
                var y = filteredByThreadNum.stream().map(r -> r.speedup).toList();
                plt.plot().add(x, y).label(a + " " + threadNum);
            }
        }
        plt.legend().loc(LEGEND_POSITION);
        plt.xlabel(X_LABEL);
        plt.ylabel(SPEEDUP_Y_LABEL);
        plt.savefig(SPEEDUP_PLOT_FILE);
        plt.show();
    }



    private ArrayList<AlgorithmResult> readStatistic() throws IOException {
        var line = "";
        BufferedReader br = new BufferedReader(new FileReader(FILE_NAME));
        var results = new ArrayList<AlgorithmResult>();
        while ((line = br.readLine()) != null) {
            String[] row = line.split(DELIMETER);
            results.add(
                new AlgorithmResult(
                    Long.parseLong(row[3]),
                    Integer.parseInt(row[1]),
                    Integer.parseInt(row[2]),
                    Double.parseDouble(row[4]),
                    row[0]
                )
            );
        }
        return results;
    }

    public void testAlgorithm(int[] threadNums, int[] matrixSizes) throws IOException {
        var file = new File( FILE_NAME);
        var results = new FileOutputStream(file);
        var matrixFactory = new Matrix2DFactory();
        for(var size : matrixSizes) {
            var first = matrixFactory.getRandom(size, size, minVal, maxVal);
            var second = matrixFactory.getRandom(size, size, minVal, maxVal);
            var threadTimeOne = 0L;
            {
                var startTime = System.currentTimeMillis();
                first.getMul(second);
                threadTimeOne = System.currentTimeMillis() - startTime;
                var result = new AlgorithmResult(
                    threadTimeOne, 1, size,
                    threadTimeOne / (double) threadTimeOne , "General").toString();
                System.out.println(result);
                results.write((result + "\n").getBytes());
            }
            for(var i = 0; i < 2; ++i) {
                for(var threadsNum : threadNums) {
                    var algName = "";
                    MultiplyAlgo algo;
                    if(i == 0) {
                        algName = BlockStripedAlgorithm.class.getSimpleName();
                        algo = new BlockStripedAlgorithm();
                    } else {
                        algName = FoxAlgorithm.class.getSimpleName();
                        algo = new FoxAlgorithm();
                    }
                    var startTime = System.currentTimeMillis();
                    algo.multiply(threadsNum, first, second);
                    var duration = System.currentTimeMillis() - startTime;
                    var result = new AlgorithmResult(
                            duration, threadsNum, size,
                            threadTimeOne / (double) duration , algName).toString();
                    System.out.println(result);
                    results.write((result + "\n").getBytes());
                }
            }
        }
    }
}
