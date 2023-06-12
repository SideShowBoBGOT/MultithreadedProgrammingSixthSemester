package org.LabExercises.Exercise4;

import java.io.File;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.RecursiveTask;
import java.util.ArrayList;

public class FolderTask extends RecursiveTask<ArrayList<String>> {
	private final ArrayList<FolderTask> folderTasks = new ArrayList<>();
	private final ArrayList<FileTask> fileTasks = new ArrayList<>();
	private final List<String> keyWords;
	private final File dir;

	public FolderTask(String name, List<String> keyWords) {
		this.dir = new File(name);
		this.keyWords = keyWords;
	}

	@Override
	protected ArrayList<String> compute() {
		for(var file : Objects.requireNonNull(dir.listFiles())) {
			if(file.isDirectory()) {
				var rhs = new FolderTask(file.getAbsolutePath(), keyWords);
				rhs.fork();
				folderTasks.add(rhs);
			} else {
				var rhs = new FileTask(file, keyWords);
				rhs.fork();
				fileTasks.add(rhs);
			}
		}
		var res = new ArrayList<String>();
		for(var fileTask : fileTasks) {
			if(fileTask.join()) {
				res.add(fileTask.file.getAbsolutePath());
			}
		}
		for(var folderTask : folderTasks) {
			res.addAll(folderTask.join());
		}
		return res;
	}
}
