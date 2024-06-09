package org.LabMatrixService.Controllers;

import org.LabMath.Matrixes.Matrix2DFactory;
import org.LabMatrixService.Models.ClientMultiplyRequest;
import org.LabMatrixService.Services.MultiplyService;
import org.LabMatrixService.Models.ServerMultiplyRequest;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.http.ResponseEntity;

@RestController
@RequestMapping("/api")
public class MatrixController {

    @PostMapping("/hello")
    public ResponseEntity<?> hello() {
        return ResponseEntity.ok("Hello World!");
    }

    @PostMapping("/multiply_server")
    public ResponseEntity<?> multiplyServer(@RequestBody ServerMultiplyRequest request) {
        System.out.println(request.toString());

        var matrixFactory = new Matrix2DFactory();

        var minVal = 0;
        var maxVal = 10;

        var first = matrixFactory.getRandom(request.size(), request.size(), minVal, maxVal);
        var second = matrixFactory.getRandom(request.size(), request.size(), minVal, maxVal);

        return ResponseEntity.ok(MultiplyService.solve(request.algType(), request.threadsNum(), first, second));
    }

    @PostMapping("/multiply_client")
    public ResponseEntity<?> multiplyClient(@RequestBody ClientMultiplyRequest request) {
        return ResponseEntity.ok(MultiplyService.solve(request.algType(), request.threadsNum(), request.first(), request.second()));
    }
}
