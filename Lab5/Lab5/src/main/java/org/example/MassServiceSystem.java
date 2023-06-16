package org.example;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;

import org.apache.logging.log4j.Logger;

public class MassServiceSystem {
	private static final String TASK_FINISHED = "[TASK EXECUTED][RUNNING TIME: %d]";
	private static final String TASK_TRANSFERRED_TO_QUEUE = "[TASK TRANSFERRED TO QUEUE]";
	private static final String TASK_TAKEN_FROM_QUEUE = "[TASK TAKEN FROM QUEUE][WAITING TIME: %d]";
	private static final String MEAN_QUEUE_LENGTH = "[MEAN QUEUE LENGTH: %d]";

	private final int maxWaitedTasks;
	private final int maxRunningTasks;

	private final Logger logger;

	private final ExecutorService executor;

	private final ArrayList<MSSTask> waitedTasks = new ArrayList<>();
	private final ArrayList<MSSTask> runningTasks = new ArrayList<>();
	private final ArrayList<MSSTask> allTasks = new ArrayList<>();

	private final AtomicBoolean isRunning = new AtomicBoolean(true);

	private final long startTime = System.currentTimeMillis();
	private long prevTime = System.currentTimeMillis();
	private long currentTime = System.currentTimeMillis();
	private long queueTimeSum = 0;

	public MassServiceSystem(int maxRunnigTasks, int maxWaitedTasks, Logger logger) {
		this.executor = Executors.newFixedThreadPool(maxRunnigTasks);
		this.maxWaitedTasks = maxWaitedTasks;
		this.maxRunningTasks = maxRunnigTasks;
		this.logger = logger;
		var workerThread = new Thread(this::work);
		workerThread.start();
	}

	public void addTask(MSSTask task) {
		if(runningTasks.size() < maxRunningTasks) {
			runningTasks.add(task);
			executor.execute(task);
		} else if(waitedTasks.size() < maxWaitedTasks) {
			logger.info(TASK_TRANSFERRED_TO_QUEUE);
			task.startTimeWaiting.set(System.currentTimeMillis());
			waitedTasks.add(task);
		} else {
			task.cancel();
		}
		allTasks.add(task);
	}

	private void work() {
		while(isRunning.get()) {
			currentTime = System.currentTimeMillis();
			updateTasks();
			prevTime = currentTime;
		}
		executor.shutdown();
	}

	private void updateTasks() {
		for(var task : runningTasks) {
			if(task.getStatus() != MSSTaskStatus.Finished) continue;

			logger.info(TASK_FINISHED.formatted(task.getTimeRunning()));

			runningTasks.remove(task);

			if(waitedTasks.isEmpty()) continue;

			var newTask = waitedTasks.remove(0);
			newTask.endTimeWaiting.set(System.currentTimeMillis());

			logger.info(TASK_TAKEN_FROM_QUEUE.formatted(newTask.getTimeWaiting()));

			runningTasks.add(newTask);
			executor.execute(newTask);
		}
	}

	private void updateQueueMeanTime() {
		queueTimeSum += waitedTasks.size() * (currentTime - prevTime);
		var mean = queueTimeSum / (currentTime - startTime);
		logger.info(MEAN_QUEUE_LENGTH.formatted(mean));
	}

	public void shutdown() {
		isRunning.set(false);
	}
}
