package org.example.LabMatrixService.Controllers;

import org.example.LabMath.Matrixes.Matrix2DFactory;
import org.example.LabMatrixService.Models.*;
import org.example.LabMatrixService.Services.MultiplyService;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api")
public class APIController {
	private static final int MIN_VAL = 0;
	private static final int MAX_VAL = 1;
	private static final int DEFAULT_SEED = 0;

	@GetMapping("/clientMultiply")
	public ResponseEntity<Result> clientMultiply(@RequestBody ClientMultiplyRequest request) throws Exception {
		var result = MultiplyService.solve(
			request.algType(), request.threadsNum(),
			request.first().getMat(), request.second().getMat());
		return ResponseEntity.ok(result);
	}

	@GetMapping("/serverMultiply")
	public ResponseEntity<Result> serverMultiply(
		@RequestBody ServerMultiplyRequest request) throws Exception {

		var factory = new Matrix2DFactory();
		var first = factory.getRandom(request.rows(), request.cols(),
			MIN_VAL, MAX_VAL, DEFAULT_SEED);
		var second = factory.getRandom(request.rows(), request.cols(),
			MIN_VAL, MAX_VAL, DEFAULT_SEED);
		var result = MultiplyService.solve(
			request.algType(), request.threadsNum(), first, second);
		return ResponseEntity.ok(result);
	}
}