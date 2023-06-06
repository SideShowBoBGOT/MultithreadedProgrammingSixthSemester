package org.example.ThirdExercise;

import java.util.ArrayList;
import java.util.Iterator;

public class Group implements Iterable<Student> {
    private final ArrayList<Student> students;
    private final String name;

    public Group(String name, int totalStudents) {
        this.name = name;
        this.students = new ArrayList<>(totalStudents);
        for(var i = 0; i < totalStudents; i++) {
            students.add(new Student("Student " + i, this));
        }
    }

    public String getName() {
        return name;
    }

    public static synchronized void printGrades(Group g) {
        for(var s : g) {
            s.printGrades();
        }
    }

    @Override
    public Iterator<Student> iterator() {
        return students.iterator();
    }
}
