package org.example;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.stream.Stream;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class MassServiceSystem {
	private static final String TASK_FINISHED = "[TASK EXECUTED][RUNNING TIME: %d]";
	private static final String TASK_TRANSFERRED_TO_QUEUE = "[TASK TRANSFERRED TO QUEUE]";
	private static final String TASK_TAKEN_FROM_QUEUE = "[TASK TAKEN FROM QUEUE][WAITING TIME: %d]";
	private static final String MEAN_QUEUE_LENGTH = "[MEAN QUEUE LENGTH: %f]";
	private static final String CANCELLATION_PROBABILITY = "[CANCELLATION PROBABILITY: %f]";

	private static final int LOG_DELAY_MILLIS = 2;

	private final int maxWaitedTasks;
	private final int maxRunningTasks;

	private final Logger logger;

	private final ExecutorService executor;

	private final ArrayList<MSSTask> tasks = new ArrayList<>();

	private final AtomicBoolean isRunning = new AtomicBoolean(true);

	private final long startTime = System.currentTimeMillis();
	private long prevTime = System.currentTimeMillis();
	private long currentTime = System.currentTimeMillis();

	private double queueTimeSum = 0;
	private double cancellationTimeSum = 0;

	public static void main(String[] args) {
		var minSleepTime = 1;
		var maxSleepTime = 5;
		var totalTasks = 10000000;
		var maximumRunningTasks = 10;
		var maximumWaitedTasks = 10;
		var logger = LogManager.getLogger("MassServiceSystem");
		var mss = new MassServiceSystem(maximumRunningTasks, maximumWaitedTasks, logger);
		for(var i = 0; i < totalTasks; ++i) {
			mss.addTask(new MSSTask(minSleepTime, maxSleepTime));
		}
	}

	public MassServiceSystem(int maxRunningTasks, int maxWaitedTasks, Logger logger) {
		this.executor = Executors.newFixedThreadPool(maxRunningTasks);
		this.maxWaitedTasks = maxWaitedTasks;
		this.maxRunningTasks = maxRunningTasks;
		this.logger = logger;
		var workerThread = new Thread(this::work);
		workerThread.start();
	}

	public void addTask(MSSTask task) {
		var sizes = getStatusSizes(MSSTaskStatus.Running, MSSTaskStatus.Waiting);;
		synchronized(tasks) {
			tasks.add(task);
		}
		synchronized(executor) {
			if(sizes[0] < maxRunningTasks || sizes[1] < maxWaitedTasks) {
				executor.execute(task);
				return;
			}
		}
		task.cancel();
	}
	private void work() {
		while(isRunning.get()) {
			currentTime = System.currentTimeMillis();

			if(currentTime - prevTime < LOG_DELAY_MILLIS) {
				continue;
			}

			var sizes = getStatusSizes(MSSTaskStatus.Waiting, MSSTaskStatus.Cancelled);

			queueTimeSum += sizes[0] * (currentTime - prevTime);
			var meanWaiting = queueTimeSum / (currentTime - startTime);
			logger.info(MEAN_QUEUE_LENGTH.formatted(meanWaiting));

			cancellationTimeSum += sizes[1] * (currentTime - prevTime);
			var meanCancellation = cancellationTimeSum / (currentTime - startTime);

			var prob = meanCancellation / getTasksSize();
			logger.info(CANCELLATION_PROBABILITY.formatted(prob));

			prevTime = currentTime;
		}
		synchronized(executor) {
			executor.shutdown();
		}
	}

	private long[] getStatusSizes(MSSTaskStatus... statuses) {
		synchronized(tasks) {
			return Stream.of(statuses).mapToLong(
				s -> tasks.stream().filter(t -> t.getStatus() == s).count()
			).toArray();
		}
	}

	private int getTasksSize() {
		synchronized(tasks) {
			return tasks.size();
		}
	}

	public void shutdown() {
		isRunning.set(false);
	}
}