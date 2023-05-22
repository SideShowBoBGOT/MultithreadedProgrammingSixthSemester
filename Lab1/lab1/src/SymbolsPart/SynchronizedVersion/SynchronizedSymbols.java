package SymbolsPart.SynchronizedVersion;

public class SynchronizedSymbols {
    public static void main(String[] args) throws InterruptedException {
        var linesPrinter = new SynchronizedLinesPrinter();

        var verticalPrinter = new SynchronizedVerticalPrinter(linesPrinter);
        var horizontalPrinter = new SynchronizedHorizontalPrinter(linesPrinter);

        var verticalPrinterThread = new Thread(verticalPrinter::printVerticalLine);
        var horizontalPrinterThread = new Thread(horizontalPrinter::printHorizontalLine);

        verticalPrinterThread.start();
        horizontalPrinterThread.start();

        verticalPrinterThread.join();
        horizontalPrinterThread.join();
    }
}
