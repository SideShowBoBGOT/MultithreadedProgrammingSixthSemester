package Exercise;

import java.util.LinkedList;
import java.util.Queue;

public class BufferExample {
    private static final int BUFFER_SIZE = 10;
    private static final Queue<Object> buffer = new LinkedList<>();

    public static void main(String[] args) {
        Thread threadA = new Thread(new Producer(), "Producer A");
        Thread threadB = new Thread(new Producer(), "Producer B");
        Thread threadC = new Thread(new Consumer(), "Consumer");

        threadA.start();
        threadB.start();
        threadC.start();
    }

    static class Producer implements Runnable {
        @Override
        public void run() {
            while (true) {
                produceItem();
            }
        }

        private void produceItem() {
            synchronized (buffer) {
                while (buffer.size() == BUFFER_SIZE) {
                    try {
                        buffer.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                Object item = new Object();
                buffer.add(item);
                System.out.println(Thread.currentThread().getName() + " produced item. Buffer size: " + buffer.size());
                buffer.notifyAll();
            }
        }
    }

    static class Consumer implements Runnable {
        @Override
        public void run() {
            while (true) {
                consumeItem();
            }
        }

        private void consumeItem() {
            synchronized (buffer) {
                while (buffer.isEmpty()) {
                    try {
                        buffer.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                Object item = buffer.remove();
                System.out.println(Thread.currentThread().getName() + " consumed item. Buffer size: " + buffer.size());
                buffer.notifyAll();
            }
        }
    }
}