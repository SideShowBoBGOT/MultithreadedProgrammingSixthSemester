package org.example.SecondExercise;

import java.util.Random;

public class Consumer implements Runnable {
    private final Drop drop;

    public Consumer(Drop drop) {
        this.drop = drop;
    }

    public void run() {
        Random random = new Random();
        for(var message = drop.take(); message != 0; message = drop.take()) {
            System.out.format("MESSAGE RECEIVED: %d%n", message);
            try {
                Thread.sleep(random.nextInt(5000));
            } catch (InterruptedException ignored) {}
        }
    }
}
