package org.example.ThirdExercise;

public class StudentsTeacherExample {
    private static final int TOTAL_TEACHERS = 4;
    private static final int TOTAL_GROUPS = 3;
    private static final int TOTAL_STUDENTS_IN_GROUP = 5;

    public static void main(String[] args) {
        var groups = new Group[TOTAL_GROUPS];
        for(var i = 0; i < TOTAL_GROUPS; i++) {
            groups[i] = new Group("Group " + i, TOTAL_STUDENTS_IN_GROUP);
        }

        var teachers = new TeacherThread[TOTAL_TEACHERS];
        for(var i = 0; i < TOTAL_TEACHERS; i++) {
            teachers[i] = new TeacherThread(groups);
            teachers[i].start();
        }

        for(var i = 0; i < TOTAL_TEACHERS; i++) {
            try {
                teachers[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
