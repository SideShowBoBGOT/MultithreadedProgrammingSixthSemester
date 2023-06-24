package org.example.LabUtils;

import javafx.util.Pair;
import java.util.concurrent.Callable;

public class Profiler {
	public <T> Pair<T, Long> performBenchmark(Callable<T> func) throws Exception {
		var startTime = System.nanoTime();
		var result = func.call();
		var duration = System.nanoTime() - startTime;
		return new Pair<>(result, duration);
	}
}
