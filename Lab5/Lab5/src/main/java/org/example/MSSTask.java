package org.example;

import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;

public class MSSTask implements Runnable {
	private static final long SLEEP_TIME_MIN = 50;
	private static final long SLEEP_TIME_MAX = 100;

	private final AtomicReference<MSSTaskStatus> status = new AtomicReference<>(MSSTaskStatus.Waiting);

	private final AtomicLong startTimeRunning = new AtomicLong(0);
	private final AtomicLong endTimeRunning = new AtomicLong(0);

	public final AtomicLong startTimeWaiting = new AtomicLong(0);
	public final AtomicLong endTimeWaiting = new AtomicLong(0);

	public MSSTask() {}

	@Override
	public void run() {
		status.set(MSSTaskStatus.Running);

		startTimeRunning.set(System.currentTimeMillis());
		try {
			Thread.sleep(genSleepTime());
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		endTimeRunning.set(System.currentTimeMillis());
		status.set(MSSTaskStatus.Finished);
	}

	private long genSleepTime() {
		return (long) (Math.random() * (SLEEP_TIME_MAX - SLEEP_TIME_MIN) + SLEEP_TIME_MIN);
	}

	public long getTimeRunning() {
		return endTimeRunning.get() - startTimeRunning.get();
	}

	public long getTimeWaiting() {
		return endTimeWaiting.get() - startTimeWaiting.get();
	}

	public MSSTaskStatus getStatus() {
		return status.get();
	}

	public void cancel() {
		status.set(MSSTaskStatus.Cancelled);
	}

}
