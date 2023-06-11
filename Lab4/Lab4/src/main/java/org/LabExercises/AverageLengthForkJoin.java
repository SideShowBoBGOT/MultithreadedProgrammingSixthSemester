package org.LabExercises;

import org.glassfish.grizzly.utils.Pair;

import java.util.Arrays;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.RecursiveTask;

public class AverageLengthForkJoin extends RecursiveTask<Pair<Integer, Integer>> {
	private final String[][] tokens;
	private final int index;

	public AverageLengthForkJoin(int index, String[][] tokens) {
		this.tokens = tokens;
		this.index = index;
	}

	@Override
	protected Pair<Integer, Integer> compute() {
		if(index >= tokens.length) return new Pair<>(0, 0);

		var rhs = new AverageLengthForkJoin(index + 1, tokens);
		rhs.fork();

		try {
			var other = rhs.get();
			var total = Arrays.stream(tokens[index]).mapToInt(String::length).sum();
			other.setFirst(total + other.getFirst());
			other.setSecond(other.getSecond() + tokens[index].length);
			return other;
		} catch(InterruptedException | ExecutionException e) {
			throw new RuntimeException(e);
		}
	}
}
