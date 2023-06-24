package org.example.LabMatrixService.Controllers;

import org.example.LabMath.Matrixes.Matrix2D;
import org.example.LabMath.Matrixes.Matrix2DFactory;
import org.example.LabMatrixService.Models.*;
import org.example.LabMatrixService.Services.MultiplyService;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import jakarta.validation.constraints.NotNull;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api")
public class APIController {
	private static final int MIN_VAL = 0;
	private static final int MAX_VAL = 1;
	private static final int DEFAULT_SEED = 0;

	@GetMapping("/multiply")
	public ResponseEntity<Result> multiply(@RequestParam AlgType algType, @RequestParam Matrix2D first,
										   @RequestParam Matrix2D second, @RequestParam int threadsNum) throws Exception {
		var result = MultiplyService.solve(algType, threadsNum, first, second);
		return ResponseEntity.ok(result);
	}

	@GetMapping("/randomMultiply")
	public ResponseEntity<Result> randomMultiply(
		@RequestParam AlgType algType, @RequestParam int rows, @RequestParam int cols,
		@RequestParam int seed, @RequestParam int threadsNum) throws Exception {

		var factory = new Matrix2DFactory();
		var first = factory.getRandom(rows, cols,
			MIN_VAL, MAX_VAL, DEFAULT_SEED);
		var second = factory.getRandom(rows, cols,
			MIN_VAL, MAX_VAL, DEFAULT_SEED);
		var result = MultiplyService.solve(algType, threadsNum, first, second);
		return ResponseEntity.ok(result);
	}

	@GetMapping
	public ResponseEntity<String> func(@RequestBody MatrixJSON algType) {
		return ResponseEntity.ok("CABAN");
	}
}