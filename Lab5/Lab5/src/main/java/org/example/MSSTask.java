package org.example;

public class MSSTask implements Runnable {
	private static final long SLEEP_TIME = 1000;
	public MSSTaskStatus status = MSSTaskStatus.Waiting;
	public long startTimeQueue = -1;
	public long endTimeQueue = -1;

	public MSSTask() {}

	@Override
	public void run() {
		status = MSSTaskStatus.Running;
		startTimeQueue = System.currentTimeMillis();
		try {
			Thread.sleep(SLEEP_TIME);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		endTimeQueue = System.currentTimeMillis();
		status = MSSTaskStatus.Finished;
	}
}
