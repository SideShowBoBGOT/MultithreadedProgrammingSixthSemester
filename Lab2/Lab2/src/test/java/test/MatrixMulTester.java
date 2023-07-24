package test;

import org.LabMath.Matrixes.Matrix2DFactory;
import org.MultiplicationAlgorithms.BlockStriped.BlockStripedAlgorithm;
import org.MultiplicationAlgorithms.Fox.FoxAlgorithm;

public class MatrixMulTester {
	private static final int minVal = -10;
	private static final int maxVal = 10;

	public static void main(String[] args) {
		var matrixSizes = new int[]{10, 20};
		var matrixFactory = new Matrix2DFactory();
		for(var size : matrixSizes) {
			var first = matrixFactory.getRandom(size, size, minVal, maxVal);
			var second = matrixFactory.getRandom(size, size, minVal, maxVal);
			var generalRes = first.getMul(second);
			var striped = new BlockStripedAlgorithm();
			var fox = new FoxAlgorithm();

			var foxRes = fox.multiply(4, first, second);
			System.out.println(generalRes);
			System.out.println(foxRes);
			var stripedRes = striped.multiply(4, first, second);
			System.out.println(stripedRes);
			System.out.println(generalRes.compareTo(foxRes) == 0);
			System.out.println(foxRes.compareTo(stripedRes) == 0);
			System.out.println(generalRes.compareTo(stripedRes) == 0);
		}
	}
}
