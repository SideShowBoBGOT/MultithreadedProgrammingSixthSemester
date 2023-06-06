package org.example.ThirdExercise;

import java.util.ArrayList;
import java.util.Arrays;

public class Student {
    private final ArrayList<Integer> grades = new ArrayList<>();
    private final String name;
    private final Group group;

    public Student(String name, Group group) {
        this.group = group;
        this.name = name;
    }

    public synchronized void addGrade(int grade) {
        grades.add(grade);
    }

    public synchronized void printGrades() {
        System.out.println(group.getName() + " " + name + ": ");
        grades.stream().limit(grades.size() - 1).toList().forEach(g -> System.out.print(g + ","));
        System.out.print(grades.get(grades.size() - 1) + "\n");
    }
}
