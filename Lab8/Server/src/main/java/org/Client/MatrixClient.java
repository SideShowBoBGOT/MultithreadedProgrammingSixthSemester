package org.Client;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.LabMath.Matrixes.Matrix2D;
import org.LabMath.Matrixes.Matrix2DFactory;
import org.LabMatrixService.Models.AlgType;
import org.LabMatrixService.Models.ClientMultiplyRequest;
import org.LabMatrixService.Models.ServerMultiplyRequest;

import java.io.FileWriter;
import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.net.http.HttpRequest.BodyPublishers;
import java.net.http.HttpResponse.BodyHandlers;
import java.util.ArrayList;
import java.util.List;

public class MatrixClient {

    public static void main(String[] args) throws Exception {
        var client = HttpClient.newHttpClient();

        var threadsNums = new int[] {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
//        var matrixSizes = new int[] {100, 300, 500, 700, 1000, 1200, 1400, 1500, 1700, 1800, 2000};
        var matrixSizes = new int[] {200, 400, 600};

        // String header = "MillisServer,MillisClient,Size,ThreadsNum,AlgType\n";

        try (FileWriter fileWriter = new FileWriter("records.csv")) {
            // fileWriter.append(header);

            for(var size : matrixSizes) {
                var matCreateStat = createMatrices(size);
                for(var threadsNum : threadsNums) {
                    for(var algType : AlgType.values()) {
                        var clientReqMillis = sendClientMultiply(client, algType,
                                matCreateStat.first, matCreateStat.second, threadsNum);
                        var serverReqMillis = sendServerMultiply(client, algType, threadsNum, size);
                        fileWriter.append(String.valueOf(serverReqMillis))
                                .append(",")
                                .append(String.valueOf(clientReqMillis + matCreateStat.millis()))
                                .append(",")
                                .append(String.valueOf(size))
                                .append(",")
                                .append(String.valueOf(threadsNum))
                                .append(",")
                                .append(algType.toString())
                                .append("\n");
                    }
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private record MatrixCreationStat(Matrix2D first, Matrix2D second, long millis) {}

    static MatrixCreationStat createMatrices(int size) {
        var minVal = 1;
        var maxVal = 10;
        var matrixFactory = new Matrix2DFactory();

        var matrixCreationStart = System.currentTimeMillis();
        var first = matrixFactory.getRandom(size, size, minVal, maxVal);
        var second = matrixFactory.getRandom(size, size, minVal, maxVal);
        var matrixCreationDur = System.currentTimeMillis() - matrixCreationStart;

        return new MatrixCreationStat(first, second, matrixCreationDur);
    }

    static long sendClientMultiply(HttpClient client, AlgType algType,
                            Matrix2D first, Matrix2D second, int threadsNum)
            throws IOException, InterruptedException {
        var cur = System.currentTimeMillis();

        var clientReq = new ClientMultiplyRequest(
                algType, first, second, threadsNum
        );
        ObjectMapper objectMapper = new ObjectMapper();
        String jsonRequest = objectMapper.writeValueAsString(clientReq);

        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create("http://localhost:8080/api/multiply_client"))
                .header("Content-Type", "application/json")
                .POST(BodyPublishers.ofString(jsonRequest))
                .build();

        client.send(request, BodyHandlers.ofString());

        return System.currentTimeMillis() - cur;
    }

    static long sendServerMultiply(HttpClient client, AlgType algType, int threadsNum, int size)
            throws IOException, InterruptedException {
        var cur = System.currentTimeMillis();

        var clientReq = new ServerMultiplyRequest(algType, threadsNum, size);
        ObjectMapper objectMapper = new ObjectMapper();
        String jsonRequest = objectMapper.writeValueAsString(clientReq);

        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create("http://localhost:8080/api/multiply_server"))
                .header("Content-Type", "application/json")
                .POST(BodyPublishers.ofString(jsonRequest))
                .build();

        client.send(request, BodyHandlers.ofString());

        return System.currentTimeMillis() - cur;
    }
}
