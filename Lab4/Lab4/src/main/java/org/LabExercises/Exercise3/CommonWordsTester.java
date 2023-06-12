package org.LabExercises.Exercise3;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Arrays;

public class CommonWordsTester {
	private static final Logger LOGGER = LogManager.getLogger("Exercise3");
	private static final String FILE_ONE = "ResourcesExercise3/1.txt";
	private static final String FILE_TWO = "ResourcesExercise3/2.txt";

	public static void main(String[] args) {
		testProfile();
		testCorrectness();
	}

	private static void testProfile() {
		var textAnalyzer = TextAnalyzer.getInstance();

		var builder = new StringBuilder();

		var singleStartTime = System.currentTimeMillis();
		textAnalyzer.getCommonWords(FILE_ONE, FILE_TWO);
		var singleDuration = System.currentTimeMillis() - singleStartTime;

		builder.append("[SINGLE DURATION: ").append(singleDuration).append("]");

		var startTime = System.currentTimeMillis();
		textAnalyzer.getCommonWordsForkJoin(FILE_ONE, FILE_TWO);
		var duration = System.currentTimeMillis() - startTime;

		builder.append("[FORK JOIN DURATION: ").append(duration).append("]");
		builder.append("[FORK JOIN EFFICIENCY: ").append((double) singleDuration / duration).append("]");

		LOGGER.info(builder.toString());
	}

	private static void testCorrectness() {
		var textAnalyzer = TextAnalyzer.getInstance();
		var res = textAnalyzer.getCommonWords(FILE_ONE, FILE_TWO);
		var builder = new StringBuilder();
		builder.append("[COMMON WORDS: ");
		Arrays.stream(res).forEach(s -> builder.append(s).append(" "));
		builder.append("]");
		LOGGER.info(builder.toString());
	}
}
