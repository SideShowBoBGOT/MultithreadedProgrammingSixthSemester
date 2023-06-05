package org.example.FirstExercise.AtomicVersion;

import java.util.Arrays;
import java.util.concurrent.atomic.AtomicIntegerArray;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;
import java.util.concurrent.atomic.AtomicReferenceFieldUpdater;

class Bank {
    private static final int NUMBER_TEST = 10000;
    private static final String TRANSACTIONS_SUM = "Transactions: %d Sum: %d\n";
    private final AtomicIntegerArray accounts;
    private final AtomicLong totalTransacts = new AtomicLong(0);

    public Bank(int n, int initialBalance){
        accounts = new AtomicIntegerArray(n);
        for(var i = 0; i < n; i++) accounts.set(i, initialBalance);
    }

    public static void main(String[] args) {
        var b = new AtomicReference<Bank>(new Bank(10, 10000));
    }

    public void transfer(int from, int to, int amount) {
        accounts.addAndGet(from, -amount);
        accounts.addAndGet(to, amount);
        if(totalTransacts.incrementAndGet() % NUMBER_TEST == 0) test();
    }

    public void test() {
        var strArray = accounts.toString();
        var sum = Arrays.stream(strArray.substring(1, strArray.length() - 1).split(", "))
                .map(Integer::parseInt).mapToInt(Integer::intValue).sum();
        System.out.printf(TRANSACTIONS_SUM, totalTransacts.get(), sum);
    }

    public int size() {
        return accounts.length();
    }
}
