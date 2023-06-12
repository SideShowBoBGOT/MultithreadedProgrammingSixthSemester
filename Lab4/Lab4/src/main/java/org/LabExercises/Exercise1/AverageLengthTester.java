package org.LabExercises.Exercise1;

import org.LabExercises.Exercise3.TextAnalyzer;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;

public class AverageLengthTester {
	private static final Logger LOGGER = LogManager.getLogger("Exercise1");
	private static final String FILE_ONE = "Exercise1_1.txt";
	private static final String FILE_TWO = "Exercise1_2.txt";

	public static void main(String[] args) throws IOException {
		testProfile(FILE_ONE);
		testProfile(FILE_TWO);
	}

	private static void testProfile(String fileName) throws IOException {
		var textAnalyzer = TextAnalyzer.getInstance();

		var builder = new StringBuilder();
		builder.append("[FILE: ").append(fileName).append("]");

		var singleStartTime = System.currentTimeMillis();
		var singleAverage = textAnalyzer.getAverageLength(fileName);
		var singleDuration = System.currentTimeMillis() - singleStartTime;

		builder.append("[SINGLE DURATION: ").append(singleDuration).append("]");
		builder.append("[SINGLE AVERAGE: ").append(singleAverage).append("]");

		var startTime = System.currentTimeMillis();
		var average = textAnalyzer.getAverageLengthForkJoin(fileName);
		var duration = System.currentTimeMillis() - startTime;

		builder.append("[FORK JOIN AVERAGE: ").append(average).append("]");
		builder.append("[FORK JOIN DURATION: ").append(duration).append("]");
		builder.append("[FORK JOIN EFFICIENCY: ").append((double) singleDuration / duration).append("]");

		LOGGER.info(builder.toString());
	}
}
