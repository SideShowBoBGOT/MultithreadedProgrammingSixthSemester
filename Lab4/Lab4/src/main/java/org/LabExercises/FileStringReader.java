package org.LabExercises;

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

public class FileStringReader {
	private static final String SENTENCE_MODEL_PATH = "opennlp-en-ud-ewt-sentence-1.0-1.9.3.bin";
	private static final String TOKEN_MODEL_PATH = "opennlp-en-ud-ewt-tokens-1.0-1.9.3.bin";
	private static final String FILE_ONE = "Exercise1_1.txt";
	private static final String FILE_TWO = "Exercise1_2.txt";
	private static final String NOT_VALID_SYMBOLS = "[^a-zA-Z\s\n]";
	private static final String WHITE_SPACE = " ";
	private static final String LINE_FEED = "\n";
	private static final String EMPTY_SPACE = "";
	private final ForkJoinPool pool = ForkJoinPool.commonPool();
	private SentenceDetectorME sentenceDetector;
	private TokenizerME tokenizer;
	private static FileStringReader fileStringReader;

	public static void main(String[] args) throws FileNotFoundException {
		System.out.println(FileStringReader.getInstance().getAverageLength(FILE_ONE));
	}

	public static FileStringReader getInstance() {
		if (fileStringReader == null) {
			return new FileStringReader();
		}
		return fileStringReader;
	}

	private FileStringReader() {
		try {
			var sentModelIn = new FileInputStream(SENTENCE_MODEL_PATH);
			var sentModel = new SentenceModel(sentModelIn);
			sentenceDetector = new SentenceDetectorME(sentModel);
			var tokModelIn = new FileInputStream(TOKEN_MODEL_PATH);
			var tokModel = new TokenizerModel(tokModelIn);
			tokenizer = new TokenizerME(tokModel);
		} catch(IOException e) {
			e.printStackTrace();
		}
	}

	public double getAverageLength(String filePath) throws FileNotFoundException {
		var text = readFile(filePath);
		var average = Arrays.stream(sentenceDetector.sentDetect(text))
			.map(s -> s.replaceAll(NOT_VALID_SYMBOLS, EMPTY_SPACE))
			.map(String::toLowerCase).map(s -> tokenizer.tokenize(s))
			.flatMap(Arrays::stream).mapToDouble(String::length).average();
		return average.isPresent() ? average.getAsDouble() : 0;
	}

	public double getAverageLengthForkJoin(String filePath) throws FileNotFoundException {
		var text = readFile(filePath);
		var sents = Arrays.stream(sentenceDetector.sentDetect(text))
			.toArray(String[]::new);
		var res = new AverageLengthForkJoin(0, sents, tokenizer);
		pool.invoke(res);
		return (double) res.join().getFirst() / res.join().getSecond();
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
