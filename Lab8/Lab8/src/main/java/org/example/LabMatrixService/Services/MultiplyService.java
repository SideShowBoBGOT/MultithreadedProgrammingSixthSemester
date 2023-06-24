package org.example.LabMatrixService.Services;

import org.example.Algorithms.BlockStripedAlgorithm;
import org.example.Algorithms.FoxAlgorithm;
import org.example.LabMatrixService.Models.AlgType;
import org.example.LabMatrixService.Models.Result;
import org.example.LabMath.Matrixes.Matrix2D;
import org.example.LabUtils.Profiler;

import java.util.ArrayList;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class MultiplyService {
	public MultiplyService() {}

	public static Result solve(AlgType algType, int threadsNum, Matrix2D first, Matrix2D second) throws Exception {
		var data = new Profiler().performBenchmark(() -> switch(algType) {
				case BlockStriped -> new BlockStripedAlgorithm(threadsNum, first, second).solve();
				case Fox -> new FoxAlgorithm(threadsNum, first, second).solve();
				case Native -> first.getMul(second);
			}
		);
		return new Result(algType, threadsNum, data.getValue());
	}
}