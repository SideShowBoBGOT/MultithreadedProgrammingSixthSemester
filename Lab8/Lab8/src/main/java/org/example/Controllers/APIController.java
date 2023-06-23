package org.example.Controllers;

import org.example.Algorithms.*;
import java.util.List;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

@RestController
@RequestMapping("analyzer")
@RequiredArgsConstructor
public class APIController {

	@GetMapping("local")
	public Result getFilePathsLocal(@RequestParam Integer size) {
		List<List<Integer>> firstMatrix = MatrixUtil.generateMatrix(size, size, 1);
		List<List<Integer>> secondMatrix = MatrixUtil.generateMatrix(size, size, 1);
		return BenchmarkPerformer.performBenchmark(new BlockStripedAlgo(), firstMatrix, secondMatrix);
	}

	@PostMapping("external")
	public Result getFilePathsExternal(
		@RequestParam AlgType algoName,
		@RequestParam Integer size,
		MultipartFile fileMatrix1,
		MultipartFile fileMatrix2
	) {
		List<List<Integer>> firstMatri = MatrixUtil.getFromFile(fileMatrix1, size, size);
		List<List<Integer>> secondMatrix = MatrixUtil.getFromFile(fileMatrix2, size, size);
		return BenchmarkPerformer.performBenchmark(switch (algoName) {
			case BLOCK_STRIPED -> new BlockStripedAlgo();
			case FOX -> new FoxAlgo();
			case NATIVE -> new NativeAlgo();
		}, firstMatrix, secondMatrix);
	}

}