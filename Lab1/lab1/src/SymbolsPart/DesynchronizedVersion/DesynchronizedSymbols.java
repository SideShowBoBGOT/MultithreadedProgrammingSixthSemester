package SymbolsPart.DesynchronizedVersion;

public class DesynchronizedSymbols {
    public static void main(String[] args) throws InterruptedException {
        var counts = 100;
        var waitTime = 10;
        var thread1 = new Thread(() -> printVerticalLine(counts, waitTime));
        var thread2 = new Thread(() -> printHorizontalLine(counts, waitTime));

        thread1.start();
        thread2.start();

        thread1.join();
        thread2.join();
    }
    private static void printVerticalLine(int counts, int waitTime) {
        for (int i = 0; i < counts; i++) {
            System.out.println("|");
            try {
                Thread.sleep(waitTime);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    private static void printHorizontalLine(int counts, int waitTime) {
        for (int i = 0; i < counts; i++) {
            System.out.println("*");
            try {
                Thread.sleep(waitTime);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
