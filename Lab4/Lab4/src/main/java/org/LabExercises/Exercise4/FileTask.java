package org.LabExercises.Exercise4;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.RecursiveTask;

public class FileTask extends RecursiveTask<Boolean> {
	private final List<String> keyWords;
	public final File file;

	public FileTask(File file, List<String> keyWords) {
		this.file = file;
		this.keyWords = keyWords;
	}

	@Override
	protected Boolean compute() {
		try {
			var scanner = new Scanner(file);
			while(scanner.hasNextLine()) {
				var line = scanner.nextLine().toLowerCase();
				if(keyWords.stream().anyMatch(line::contains)) {
					return true;
				}
			}
			return false;
		} catch(FileNotFoundException e) {
			throw new RuntimeException(e);
		}
	}
}
