package org.example.LabMatrixService.Services;

import org.example.Algorithms.BlockStripedAlgorithm;
import org.example.Algorithms.FoxAlgorithm;
import org.example.LabMatrixService.Models.AlgType;
import org.example.LabMatrixService.Models.Result;
import org.example.LabMath.Matrixes.Matrix2D;
import org.example.LabUtils.Profiler;

public class MultiplyService {
	public MultiplyService() {}

	public static Result solve(AlgType algType, int threadsNum, Matrix2D first, Matrix2D second) throws Exception {
		var data = new Profiler().performBenchmark(() -> switch(algType) {
				case BLOCK_STRIPED -> new BlockStripedAlgorithm(threadsNum, first, second).solve();
				case FOX -> new FoxAlgorithm(threadsNum, first, second).solve();
				case NATIVE -> first.getMul(second);
			}
		);
		return new Result(algType, threadsNum, data.getValue());
	}
}