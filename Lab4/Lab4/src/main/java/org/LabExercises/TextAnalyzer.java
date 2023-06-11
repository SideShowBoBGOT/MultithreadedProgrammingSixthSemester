package org.LabExercises;

import opennlp.tools.cmdline.lemmatizer.LemmatizerMETool;
import opennlp.tools.lemmatizer.Lemmatizer;
import opennlp.tools.lemmatizer.LemmatizerME;
import opennlp.tools.lemmatizer.LemmatizerModel;
import opennlp.tools.postag.POSModel;
import opennlp.tools.postag.POSTaggerME;
import opennlp.tools.sentdetect.SentenceDetectorME;
import opennlp.tools.sentdetect.SentenceModel;
import opennlp.tools.tokenize.TokenizerME;
import opennlp.tools.tokenize.TokenizerModel;
import org.springframework.data.util.StreamUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.ForkJoinPool;
import java.util.stream.Stream;

public class TextAnalyzer {
	private static final String SENTENCE_MODEL_PATH = "en-sent.bin";
	private static final String TOKEN_MODEL_PATH = "en-token.bin";
	private static final String LEMMATIZER_MODEL_PATH = "en-lemmatizer.bin";
	private static final String POSTAGGER_MODEL_PATH = "en-pos-maxent.bin";
	private static final String NOT_VALID_SYMBOLS = "[^a-zA-Z\s\n]";
	private static final String WHITE_SPACE = " ";
	private static final String LINE_FEED = "\n";
	private static final String EMPTY_SPACE = "";
	private final ForkJoinPool pool = ForkJoinPool.commonPool();
	private SentenceDetectorME sentenceDetector;
	private TokenizerME tokenizer;
	private LemmatizerME lemmatizer;
	private POSTaggerME posTagger;

	private static TextAnalyzer fileStringReader;

	public static TextAnalyzer getInstance() {
		if (fileStringReader == null) {
			return new TextAnalyzer();
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
			var lemModelIn = new FileInputStream(LEMMATIZER_MODEL_PATH);
			var lemModel = new LemmatizerModel(lemModelIn);
			lemmatizer = new LemmatizerME(lemModel);
			var posModelIn = new FileInputStream(POSTAGGER_MODEL_PATH);
			var posModel = new POSModel(posModelIn);
			posTagger = new POSTaggerME(posModel);
		} catch(IOException e) {
			throw new RuntimeException(e);
		}
	}

	public String[] getCommonWords(String fileNameOne, String fileNameTwo) {
		var tokens = Stream.of(fileNameOne, fileNameTwo)
			.map(s -> getTokenStream(s).toList()).toList();
		var tagsStream = tokens.stream().map(
			s -> s.stream().map(t -> posTagger.tag(t)).toList());
		var lemmas = StreamUtils.zip(tokens.stream(), tagsStream, (tokMat, tagMat) ->
			StreamUtils.zip(tokMat.stream(), tagMat.stream(), lemmatizer::lemmatize)
				.flatMap(Arrays::stream).toList()).toList();
		var common = lemmas.get(0).stream().filter(lemmas.get(1)::contains)
			.toArray(String[]::new);
		return common;
	}

	public Stream<String[]> getTokenStream(String fileName) {
		try {
			String text = readFile(fileName);
			return Arrays.stream(sentenceDetector.sentDetect(text))
				.map(s -> s.replaceAll(NOT_VALID_SYMBOLS, EMPTY_SPACE))
				.map(s -> tokenizer.tokenize(s));
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
