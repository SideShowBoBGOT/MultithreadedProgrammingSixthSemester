package org.example.FirstExercise.LockVersion;

import java.util.Arrays;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class Bank {
    public static final int NUMBER_TEST = 10000;
    private final int[] accounts;
    private long totalTransacts = 0;
    private final ReentrantLock bankLock = new ReentrantLock();

    public Bank(int n, int initialBalance){
        accounts = new int[n];
        Arrays.fill(accounts, initialBalance);
    }

    public synchronized void transfer(int from, int to, int amount) {
        bankLock.lock();
        accounts[from] -= amount;
        accounts[to] += amount;
        totalTransacts++;
        if(totalTransacts % NUMBER_TEST == 0) {
            test();
        }
        bankLock.unlock();
    }

    public void test() {
        var sum = Arrays.stream(accounts).sum();
        System.out.println("Transactions:" + totalTransacts + " Sum: " + sum);
    }

    public int size() {
        return accounts.length;
    }
}
