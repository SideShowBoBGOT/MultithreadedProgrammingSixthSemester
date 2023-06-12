package org.LabExercises.Exercise3;

import java.util.concurrent.RecursiveAction;
import java.util.List;

public class CommonWordsForkJoin extends RecursiveAction {
	private final List<String> commonWords;
	private final List<String> first;
	private final List<String> second;
	private final int index;

	public CommonWordsForkJoin(int index, List<String> first, List<String> second, List<String> commonWords) {
		this.commonWords = commonWords;
		this.first = first;
		this.second = second;
		this.index = index;
	}

	@Override
	protected void compute() {
		if(index >= first.size()) return;

		var rhs = new CommonWordsForkJoin(index + 1, first, second, commonWords);
		rhs.fork();

		if(second.contains(first.get(index))) {
			synchronized(commonWords) {
				commonWords.add(first.get(index));
			}
		}

		rhs.join();
	}
}
