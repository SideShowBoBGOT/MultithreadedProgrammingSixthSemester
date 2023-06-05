package org.example.FirstExercise.AtomicVersion;

public class AsyncBankTest {

    public static void main(String[] args) {
        var totalAccounts = 10;
        var initialBalance = 10000;
        var b = new Bank(totalAccounts, initialBalance);
        for(var i = 0; i < totalAccounts; i++){
            var t = new TransferThread(b, i, initialBalance);
            t.setPriority(Thread.NORM_PRIORITY + i % 2);
            t.start();
        }
    }
}
