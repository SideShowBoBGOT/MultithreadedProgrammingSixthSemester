package org.LabExercises.Exercise1;

import opennlp.tools.sentdetect.SentenceDetectorME;
import opennlp.tools.sentdetect.SentenceModel;
import opennlp.tools.tokenize.TokenizerME;
import opennlp.tools.tokenize.TokenizerModel;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.ForkJoinPool;
import java.util.stream.Stream;

public class TextAnalyzer {
	private static final String SENTENCE_MODEL_PATH = "LanguageModels/en-sent.bin";
	private static final String TOKEN_MODEL_PATH = "LanguageModels/en-token.bin";
	private static final String NOT_VALID_SYMBOLS = "[^a-zA-Z\s\n]";
	private static final String LINE_FEED = "\n";
	private static final String EMPTY_SPACE = "";
	private final ForkJoinPool pool = ForkJoinPool.commonPool();
	private final SentenceDetectorME sentenceDetector;
	private final TokenizerME tokenizer;

	private static TextAnalyzer fileStringReader;

	public static TextAnalyzer getInstance() {
		if (fileStringReader == null) {
			fileStringReader = new TextAnalyzer();
		}
		return fileStringReader;
	}

	private TextAnalyzer() {
		try {
			var sentModelIn = new FileInputStream(SENTENCE_MODEL_PATH);
			var sentModel = new SentenceModel(sentModelIn);
			sentenceDetector = new SentenceDetectorME(sentModel);
			var tokModelIn = new FileInputStream(TOKEN_MODEL_PATH);
			var tokModel = new TokenizerModel(tokModelIn);
			tokenizer = new TokenizerME(tokModel);
		} catch(IOException e) {
			throw new RuntimeException(e);
		}
	}

	public Stream<String[]> getTokenStream(String fileName) {
		try {
			String text = readFile(fileName);
			return Arrays.stream(sentenceDetector.sentDetect(text))
				.map(s -> s.replaceAll(NOT_VALID_SYMBOLS, EMPTY_SPACE))
				.map(tokenizer::tokenize);
		} catch(FileNotFoundException e) {
			throw new RuntimeException(e);
		}
	}

	public double getAverageLength(String filePath) {
		var average = getTokenStream(filePath).flatMap(Arrays::stream)
			.mapToDouble(String::length).average();
		return average.isPresent() ? average.getAsDouble() : 0;
	}

	public double getAverageLengthForkJoin(String filePath) {
		var tokens = getTokenStream(filePath).toArray(String[][]::new);
		var res = pool.invoke(new AverageLengthForkJoin(0, tokens));
		return (double) res.getFirst() / res.getSecond();
	}

	public String readFile(String filePath) throws FileNotFoundException {
		var file = new File(filePath);
		var scanner = new Scanner(file);
		var builder = new StringBuilder();
		while(scanner.hasNextLine()) {
			builder.append(scanner.nextLine()).append(LINE_FEED);
		}
		return builder.toString();
	}
}
