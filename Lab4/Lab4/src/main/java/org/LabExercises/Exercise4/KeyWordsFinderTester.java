package org.LabExercises.Exercise4;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.List;

public class KeyWordsFinderTester {
	private static final Logger LOGGER = LogManager.getLogger("Exercise4");
	private static final String DIRECTORY = "ResourcesExercise4/Test";

	public static void main(String[] args) {
		testProfile();
	}

	private static void testProfile() {
		var keyWordsFinder = new KeyWordsFinder();
		var keyWords = List.of("c++", "java", "python");

		var builder = new StringBuilder();

		var singleStartTime = System.currentTimeMillis();
		keyWordsFinder.findDocuments(DIRECTORY, keyWords);
		var singleDuration = System.currentTimeMillis() - singleStartTime;

		builder.append("[SINGLE DURATION: ").append(singleDuration).append("]");

		var startTime = System.currentTimeMillis();
		keyWordsFinder.findDocumentsForkJoin(DIRECTORY, keyWords);
		var duration = System.currentTimeMillis() - startTime;

		builder.append("[FORK JOIN DURATION: ").append(duration).append("]");
		builder.append("[FORK JOIN EFFICIENCY: ").append((double) singleDuration / duration).append("]");

		LOGGER.info(builder.toString());
	}
}
