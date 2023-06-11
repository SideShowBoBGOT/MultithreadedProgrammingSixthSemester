package test;

import org.LabExercises.TextAnalyzer;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;

public class CommonWordsTester {
	private static final Logger LOGGER = LogManager.getLogger("Exercise3");
	private static final String FILE_ONE = "Exercise3_1.txt";
	private static final String FILE_TWO = "Exercise3_2.txt";

	public static void main(String[] args) throws FileNotFoundException {
		testCorrectness();
	}

	private static void testCorrectness() throws FileNotFoundException {
		var textAnalyzer = TextAnalyzer.getInstance();
		var res = textAnalyzer.getCommonWords(FILE_ONE, FILE_TWO);
		var builder = new StringBuilder();
		builder.append("[COMMON WORDS: ");
		Arrays.stream(res).forEach(s -> builder.append(s).append(" "));
		builder.append("]");
		LOGGER.info(builder.toString());
	}


}
