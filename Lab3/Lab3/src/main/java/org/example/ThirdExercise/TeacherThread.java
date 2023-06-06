package org.example.ThirdExercise;

import java.util.Arrays;

public class TeacherThread extends Thread {
    private final Group[] groups;
    public TeacherThread(Group[] groups) {
        this.groups = groups;
    }

    @Override
    public void run() {
        while(true) {
            for(var g : groups) {
                for(var s : g) {
                    s.addGrade((int) (100 * Math.random()));
                }
                Group.printGrades(g);
            }
            try {
                Thread.sleep(4000);
            } catch(InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
