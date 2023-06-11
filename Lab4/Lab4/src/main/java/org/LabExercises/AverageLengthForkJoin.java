package org.LabExercises;

import opennlp.tools.tokenize.TokenizerME;
import org.glassfish.grizzly.utils.Pair;

import java.util.Arrays;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.RecursiveTask;

public class AverageLengthForkJoin extends RecursiveTask<Pair<Integer, Integer>> {
	private static final String NOT_VALID_SYMBOLS = "[^a-zA-Z\s\n]";
	private static final String WHITE_SPACE = " ";
	private static final String EMPTY_SPACE = "";
	private final String[] sentences;
	private final TokenizerME tokenizer;
	private final int index;


	public AverageLengthForkJoin(int index, String[] sentences, TokenizerME tokenizer) {
		this.sentences = sentences;
		this.tokenizer = tokenizer;
		this.index = index;
	}

	@Override
	protected Pair<Integer, Integer> compute() {
		if(index == sentences.length) return new Pair<>(0, 0);

		var rhs = new AverageLengthForkJoin(index + 1, sentences, tokenizer);
		rhs.fork();
		var tokens = tokenizer.tokenize(sentences[index]
			.replaceAll(NOT_VALID_SYMBOLS, EMPTY_SPACE).toLowerCase());
		var other = rhs.join();
		var total = Arrays.stream(tokens).mapToInt(String::length).sum();
		other.setFirst(total + other.getFirst());
		other.setSecond(other.getSecond() + tokens.length);
		return other;
	}
}
