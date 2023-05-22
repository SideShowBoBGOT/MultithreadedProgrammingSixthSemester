package SymbolsPart.SynchronizedVersion;

public class SynchronizedHorizontalPrinter {
    private final SynchronizedLinesPrinter synchronizedLinesPrinter;
    SynchronizedHorizontalPrinter(SynchronizedLinesPrinter synchronizedLinesPrinter) {
        this.synchronizedLinesPrinter = synchronizedLinesPrinter;
    }
    public void printHorizontalLine() {
        while(true) {
            synchronizedLinesPrinter.printHorizontalLine();
        }
    }
}
