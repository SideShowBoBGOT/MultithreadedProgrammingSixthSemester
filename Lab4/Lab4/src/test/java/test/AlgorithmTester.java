package test;

import org.LabExercises.Exercise2.ExecutorAlgorithm;
import org.LabExercises.Exercise2.ForkJoinAlgorithm;
import org.LabMath.Matrixes.Matrix2D;
import org.LabMath.Matrixes.Matrix2DFactory;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.stream.IntStream;

public class AlgorithmTester {
	private static final Matrix2DFactory MATRIX_2_D_FACTORY = new Matrix2DFactory();
	private static final Logger LOGGER = LogManager.getLogger("Exercise2");
	private static final int MIN_VAL = 0;
	private static final int MAX_VAL = 10;

	private static Matrix2D getTestMatrix(int size) {
		return MATRIX_2_D_FACTORY.getRandom(size, size, MIN_VAL, MAX_VAL);
	}

	public static void main(String[] args) {
		testExecutor();
		testForkJoin();
	}

	private static void testForkJoin() {
		IntStream.of(100, 200, 300, 400).forEach(s -> {
			var mat = getTestMatrix(s);
			var startTime = System.currentTimeMillis();
			mat.getMul(mat);
			var singleDuration = System.currentTimeMillis() - startTime;

			var start = System.currentTimeMillis();
			var forkJoinAlg = new ForkJoinAlgorithm(mat, mat);
			forkJoinAlg.solve();
			var d = System.currentTimeMillis() - start;

			var builder = new StringBuilder();
			builder.append("[SIZE: ").append(s).append("]");
			builder.append("[SINGLE DURATION: ").append(singleDuration).append("]");
			builder.append("[FORK JOIN DURATION: ").append(d).append("]");
			builder.append("[FORK JOIN EFFICIENCY: ").append((double) singleDuration / d).append("]");
			LOGGER.info(builder.toString());
		});
	}

	private static void testExecutor() {
		var builder = new StringBuilder();
		IntStream.of(100, 200, 300, 400).forEach(s -> {
			var mat = getTestMatrix(s);
			var startTime = System.currentTimeMillis();
			mat.getMul(mat);
			var singleDuration = System.currentTimeMillis() - startTime;
			IntStream.of(2, 3, 4).forEach(n -> {
				builder.append("[SIZE: ").append(s).append("]");
				builder.append("[SINGLE DURATION: ").append(singleDuration).append("]");
				builder.append("[THREADS: ").append(n).append("]");

				var start = System.currentTimeMillis();
				var executorAlg = new ExecutorAlgorithm(n, mat, mat);
				executorAlg.solve();
				var d = System.currentTimeMillis() - start;

				builder.append("[EXECUTOR DURATION: ").append(d).append("]");
				builder.append("[EXECUTOR EFFICIENCY: ").append((double) singleDuration / d).append("]");
				LOGGER.info(builder.toString());
				builder.setLength(0);
			});
		});
	}
}
