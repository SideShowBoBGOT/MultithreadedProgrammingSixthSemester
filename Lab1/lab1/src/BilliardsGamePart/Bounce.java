package BilliardsGamePart;

import BilliardsGamePart.LabControllers.Application;

import javax.swing.*;

public class Bounce {
    public static void main(String[] args) {
        var frame = Application.getInstance();
        frame.init(1360, 720);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
        System.out.println("Thread name = " + Thread.currentThread().getName());
    }
}