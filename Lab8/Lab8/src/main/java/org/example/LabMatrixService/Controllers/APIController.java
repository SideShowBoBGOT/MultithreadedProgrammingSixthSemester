package org.example.LabMatrixService.Controllers;

import org.example.LabMath.Matrixes.Matrix2D;
import org.example.LabMath.Matrixes.Matrix2DFactory;
import org.example.LabMatrixService.Models.RandomMultiplyRequest;
import org.example.LabMatrixService.Models.Result;
import org.example.LabMatrixService.Services.MultiplyService;
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
	public ResponseEntity<Result> multiply(@NotNull @RequestBody MultiplyRequest request) throws Exception {
		var result = MultiplyService.solve(
			request.algType(), request.threadsNum(),
			request.first(), request.second()
		);
		return ResponseEntity.ok(result);
	}

	@GetMapping("/multiply")
	public ResponseEntity<Result> randomMultiply(@NotNull @RequestBody RandomMultiplyRequest request) throws Exception {
		var factory = new Matrix2DFactory();
		var first = factory.getRandom(request.rows(), request.cols(),
			MIN_VAL, MAX_VAL, DEFAULT_SEED);
		var second = factory.getRandom(request.rows(), request.cols(),
			MIN_VAL, MAX_VAL, DEFAULT_SEED);
		var result = MultiplyService.solve(
			request.algType(), request.threadsNum(),
			first, second
		);
		return ResponseEntity.ok(result);
	}

}