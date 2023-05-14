import LabForms.ApplicationForm;

import javax.swing.*;

public class Bounce {
    public static void main(String[] args) {
        ApplicationForm frame = new ApplicationForm();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
        System.out.println("Thread name = " + Thread.currentThread().getName());
    }
}