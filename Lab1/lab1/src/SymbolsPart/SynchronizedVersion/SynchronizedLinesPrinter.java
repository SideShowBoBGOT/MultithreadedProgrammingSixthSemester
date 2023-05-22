package SymbolsPart.SynchronizedVersion;

public class SynchronizedLinesPrinter {
    private static final int WAIT_TIME = 1000;
    private boolean isVerticalLine = true;
    SynchronizedLinesPrinter() {}
    synchronized void printVerticalLine() {
        while(isVerticalLine) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.println("|");
        isVerticalLine = true;
        try {
            Thread.sleep(WAIT_TIME);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        notify();
    }
    synchronized void printHorizontalLine() {
        while(!isVerticalLine) {
            try {
                wait();
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.println("*");
        isVerticalLine = false;
        try {
            Thread.sleep(WAIT_TIME);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        notify();
    }
}
