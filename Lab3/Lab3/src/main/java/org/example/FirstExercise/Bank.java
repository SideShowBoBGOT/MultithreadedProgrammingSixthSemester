package org.example.FirstExercise;

import java.util.Arrays;

class Bank {
    public static final int NTEST = 10000;
    private final int[] accounts;
    private long totalTransacts = 0;

    public Bank(int n, int initialBalance){
        accounts = new int[n];
        Arrays.fill(accounts, initialBalance);
    }

    public void transfer(int from, int to, int amount) {
        accounts[from] -= amount;
        accounts[to] += amount;
        totalTransacts++;
        if(totalTransacts % NTEST == 0) {
            test();
        }
    }

    public void test() {
        var sum = Arrays.stream(accounts).sum();;
        System.out.println("Transactions:" + totalTransacts + " Sum: " + sum);
    }

    public int size() {
        return accounts.length;
    }
}
