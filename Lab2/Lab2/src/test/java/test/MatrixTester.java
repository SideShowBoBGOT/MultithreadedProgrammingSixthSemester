package test;

import com.github.sh0nk.matplotlib4j.*;
import org.LabMath.Matrixes.Matrix2DFactory;
import org.MultiplicationAlgorithms.BlockStripedAlgorithm;
import org.MultiplicationAlgorithms.FoxAlgorithm;
import org.MultiplicationAlgorithms.GeneralAlgorithm;

import java.awt.*;
import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

public class MatrixTester {
    private static final int minVal = -10;
    private static final int maxVal = 10;
    private static final String FILE_NAME = "results.csv";
    private static final String DELIMETER = "\t";
    private static final String LEGEND_POSITION = "upper left";
    private static final String X_LABEL = "Matrix size";
    private static final String Y_LABEL = "Milliseconds";


    private static class AlgorithmResult {
        public final long milliseconds;
        public final int threadsNum;
        public final int size;
        public final String name;

        public AlgorithmResult(long time, int threadsNum, int size, String name) {
            this.milliseconds = time;
            this.threadsNum = threadsNum;
            this.size = size;
            this.name = name;
        }

        @Override
        public String toString() {
            return String.format("%s\t%d\t%d\t%d", name, threadsNum, size, milliseconds);
        }
    }

    public static void main(String[] args) throws PythonExecutionException, IOException {
            var tester = new MatrixTester();
//            var threadsNums = new int[] {1, 4, 10};
//            var matrixSizes = new int[] {100, 200, 300, 400, 500, 600, 700};
//            var algorithms = new GeneralAlgorithm[] {new FoxAlgorithm(), new BlockStripedAlgorithm()};
//            tester.testAlgorithm(threadsNums, matrixSizes, algorithms);

            tester.plotStatistic();
    }

    public void plotStatistic() throws PythonExecutionException, IOException {
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
                    row[0]
                )
            );
        }
        return results;
    }

    public void testAlgorithm(int[] threadNums, int[] matrixSizes, GeneralAlgorithm[] algorithms) throws IOException {
        var file = new File( FILE_NAME);
        var results = new FileOutputStream(file);
        var matrixFactory = new Matrix2DFactory();
        for(var algorithm : algorithms) {
            var algName = algorithm.getClass().getSimpleName();
            for(var threadsNum : threadNums) {
                for(var size : matrixSizes) {
                    var startTime = System.currentTimeMillis();

                    var first = matrixFactory.getRandom(size, size, minVal, maxVal);
                    var second = matrixFactory.getRandom(size, size, minVal, maxVal);

                    algorithm.setThreadsNum(threadsNum);
                    algorithm.setFirst(first);
                    algorithm.setSecond(second);

                    algorithm.solve();

                    var endTime = System.currentTimeMillis();

                    var duration = endTime - startTime;

                    var result = new AlgorithmResult(duration, threadsNum, size, algName).toString();
                    System.out.println(result);
                    results.write((result + "\n").getBytes());
                }
            }
        }
    }
}
