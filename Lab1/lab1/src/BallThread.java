public class BallThread extends Thread {
    private Ballwww b;

    public BallThread(Ballwww ball) {
        b = ball;
    }

    @Override
    public void run() {
        try {
            for(int i = 1; i < 10000; i++) {
                b.UpdateLocation();
                System.out.println("Thread name = "
                        + Thread.currentThread().getName());
                Thread.sleep(5);
            }
        } catch(InterruptedException ex) {
        }
    }
}