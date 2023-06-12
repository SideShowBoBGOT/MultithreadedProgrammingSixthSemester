package org.LabExercises.Exercise4;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.Scanner;
import java.util.concurrent.ForkJoinPool;

public class KeyWordsFinder {
	private final ForkJoinPool pool = ForkJoinPool.commonPool();

	public KeyWordsFinder() {}

	public static void main(String[] args) {
		var finder = new KeyWordsFinder();
	}

	public ArrayList<String> findDocuments(String rootDirectory, List<String> keyWords) {
		var keyWordsLower = keyWords.stream().map(String::toLowerCase).toList();
		var res = new ArrayList<String>();
		try {
			doFindDocuments(rootDirectory, keyWordsLower, res);
		} catch(FileNotFoundException e) {
			throw new RuntimeException(e);
		}
		return res;
	}

	private void doFindDocuments(String rootDirectory, List<String> keyWords, ArrayList<String> res) throws FileNotFoundException {
		var keyWordsLower = keyWords.stream().map(String::toLowerCase).toList();
		var dir = new File(rootDirectory);
		for(var file : Objects.requireNonNull(dir.listFiles())) {
			if(file.isDirectory()) {
				doFindDocuments(file.getAbsolutePath(), keyWordsLower, res);
			} else {
				var scanner = new Scanner(file);
				while(scanner.hasNextLine()) {
					var line = scanner.nextLine().toLowerCase();
					if(keyWords.stream().anyMatch(line::contains)) {
						res.add(file.getAbsolutePath());
					}
				}
			}
		}
	}

	public ArrayList<String> findDocumentsForkJoin(String rootDirectory, List<String> keyWords) {
		var keyWordsLower = keyWords.stream().map(String::toLowerCase).toList();
		return pool.invoke(new FolderTask(rootDirectory, keyWordsLower));
	}
}
