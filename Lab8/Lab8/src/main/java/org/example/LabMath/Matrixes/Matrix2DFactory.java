package org.example.LabMath.Matrixes;

public class Matrix2DFactory {
    
    public Matrix2DFactory() {}

    public static void main(String[] args) {
        var factory = new Matrix2DFactory();
        var minVal = 0;
        var maxVal = 10;
        var rows = 5;
        var cols = 6;
        var one = factory.getRandom(rows, cols, minVal, maxVal);
        var two = factory.getRandom(cols, rows, minVal, maxVal);
        var result = one.getMul(two);
        System.out.println(result);
    }

    public Matrix2D getRandom(int rows, int cols, int minVal, int maxVal) {
        var res = new Matrix2D(rows, cols);
        for(var i = 0; i < rows; ++i) {
            for(var j = 0; j < cols; ++j) {
                res.setAt(Math.random() * (maxVal - minVal) + minVal, i, j);
            }
        }
        return res;
    }
}
