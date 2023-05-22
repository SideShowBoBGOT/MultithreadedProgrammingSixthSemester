package SymbolsPart.SynchronizedVersion;

public class SynchronizedVerticalPrinter {
    private final SynchronizedLinesPrinter synchronizedLinesPrinter;
    SynchronizedVerticalPrinter(SynchronizedLinesPrinter synchronizedLinesPrinter) {
        this.synchronizedLinesPrinter = synchronizedLinesPrinter;
    }
    public void printVerticalLine() {
        while(true) {
            synchronizedLinesPrinter.printVerticalLine();
        }
    }
}
