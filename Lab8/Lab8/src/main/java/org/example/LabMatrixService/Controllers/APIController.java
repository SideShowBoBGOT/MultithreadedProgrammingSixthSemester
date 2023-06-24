package org.example.LabMatrixService.Controllers;

import org.example.LabMath.Matrixes.Matrix2DFactory;
import org.example.LabMatrixService.Models.Result;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import jakarta.validation.constraints.NotNull;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import org.example.LabMatrixService.Models.MultiplyRequest;

@RestController
@RequestMapping("/api")
public class APIController {
	private static final int MIN_VAL = 0;
	private static final int MAX_VAL = 1;
	private static final int DEFAULT_SEED = 0;

	@PostMapping("/multiply")
	public ResponseEntity<Result> multiply(@NotNull @RequestBody MultiplyRequest request) {
		var firstMatrix = new Matrix(request.firstMatrix());
		var secondMatrix = new Matrix(request.secondMatrix());

		var multiplyService = new MultiplyService(firstMatrix, secondMatrix);
		var result = multiplyService.solve(request.numberOfThreads());

		return ResponseEntity.ok(result);
	}

	@GetMapping("/multiply")
	public ResponseEntity<Result> randomMultiply(@RequestParam int rows, @RequestParam int columns,  @RequestParam int numberOfThreads) {
		var factory = new Matrix2DFactory();
		var first = factory.getRandom(rows, columns, MIN_VAL, MAX_VAL, DEFAULT_SEED);
		var second = factory.getRandom(rows, columns, MIN_VAL, MAX_VAL, DEFAULT_SEED);

		var multiplyService = new MultiplyService(firstMatrix, secondMatrix);
		var result = multiplyService.solve(numberOfThreads);

		return ResponseEntity.ok(result);
	}

	@GetMapping("/generate-matrix")
	public @ResponseBody ResponseEntity<int[][]> multiply(@RequestParam int rows, @RequestParam int columns, @RequestParam int seed) {
		var matrix = new Matrix(rows, columns, seed);

		return ResponseEntity.ok(matrix.getMatrix());
	}

}