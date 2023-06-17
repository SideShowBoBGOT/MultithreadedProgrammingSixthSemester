package org.example;

public class MSSTask implements Runnable {
	private static final String MAX_LESS_THAN_MIN = "maxSleepTime < minSleepTime";
	private final long minSleepTime;
	private final long maxSleepTime;

	private final Object statusLock = new Object();
	private MSSTaskStatus status = MSSTaskStatus.Waiting;

	public MSSTask(int minSleepTime, int maxSleepTime) {
		if(minSleepTime > maxSleepTime) {
			throw new IllegalArgumentException(MAX_LESS_THAN_MIN);
		}
		this.minSleepTime = minSleepTime;
		this.maxSleepTime = maxSleepTime;
	}

	@Override
	public void run() {
		setStatus(MSSTaskStatus.Running);

		try {
			Thread.sleep(genSleepTime());
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		setStatus(MSSTaskStatus.Finished);
	}

	private long genSleepTime() {
		return (long) (Math.random() * (maxSleepTime - minSleepTime) + minSleepTime);
	}

	private void setStatus(MSSTaskStatus status) {
		synchronized(statusLock) {
			this.status = status;
		}
	}

	public MSSTaskStatus getStatus() {
		synchronized(statusLock) {
			return status;
		}
	}

	public void cancel() {
		setStatus(MSSTaskStatus.Cancelled);
	}

}
