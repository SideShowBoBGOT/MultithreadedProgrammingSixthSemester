package Exercise;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

public class TrafficLightSimulation {
    private static final int NUM_CARS = 100;
    private static final int MAX_CARS_PASSED = 10000;
    private static final AtomicBoolean isGreenLight = new AtomicBoolean(true);
    private static final AtomicInteger carsPassed = new AtomicInteger(0);
    private static final CountDownLatch latch = new CountDownLatch(NUM_CARS);

    public static void main(String[] args) throws InterruptedException {
        ExecutorService executor = Executors.newFixedThreadPool(NUM_CARS + 1);

        executor.execute(new TrafficLight());

        for (int i = 0; i < NUM_CARS; i++) {
            executor.execute(new Car(i));
        }

        latch.await();
        executor.shutdownNow();
    }

    static class TrafficLight implements Runnable {
        @Override
        public void run() {
            while (carsPassed.get() < MAX_CARS_PASSED) {
                try {
                    Thread.sleep(70);
                    isGreenLight.set(false);
                    Thread.sleep(10);
                    Thread.sleep(40);
                    Thread.sleep(10);
                    isGreenLight.set(true);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }
    }

    static class Car implements Runnable {
        private final int carId;

        public Car(int carId) {
            this.carId = carId;
        }

        @Override
        public void run() {
            while (carsPassed.get() < MAX_CARS_PASSED) {
                if (isGreenLight.get()) {
                    go();
                    carsPassed.incrementAndGet();
                }
                try {
                    Thread.sleep(400);
                } catch (InterruptedException e) {
                    break;
                }
            }
            latch.countDown();
        }

        private void go() {
            try {
                Thread.sleep(2);
                System.out.println("Car " + carId + " passed the traffic light.");
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }
}
