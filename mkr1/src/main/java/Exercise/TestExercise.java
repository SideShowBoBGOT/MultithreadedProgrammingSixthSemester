package Exercise;

public class TestExercise {
    private enum State {
        R, W
    }

    private static volatile State state = State.R;

    public static void main(String[] args) {
        Thread threadA = new Thread(new Runnable() {
            @Override
            public void run() {
                runThreadA();
            }
        });

        Thread threadB = new Thread(new Runnable() {
            @Override
            public void run() {
                runThreadB();
            }
        });

        threadA.start();
        threadB.start();
    }

    private static void runThreadA() {
        while (true) {
            synchronized (TestExercise.class) {
                state = State.W;
                TestExercise.class.notifyAll();
            }
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (TestExercise.class) {
                state = State.R;
                TestExercise.class.notifyAll();
            }
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private static void runThreadB() {
        while (true) {
            synchronized (TestExercise.class) {
                while (state != State.R) {
                    try {
                        TestExercise.class.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                for (int i = 100; i > 0; i--) {
                    System.out.println("Countdown: " + i);
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }
}