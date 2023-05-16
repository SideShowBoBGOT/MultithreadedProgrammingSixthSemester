package LabForms;

import LabLogic.Ball;
import LabThreads.BallThread;

import javax.swing.*;
import java.awt.*;

public class ApplicationForm extends JFrame {
    private static final String Title = "Bounce program";
    private static final String ButtonStartText = "Start";
    private static final String ButtonStopText = "Stop";
    private static final String InFrameThreadName = "In Frame thread name = ";
    private final MapForm mapForm;

    public ApplicationForm(int width, int height) {
        this.setSize(width, height);
        this.setTitle(Title);
        this.mapForm = new MapForm(new LabLogic.Map(new Point(width, height)));
        SetUpLayout();
        System.out.println(InFrameThreadName + Thread.currentThread().getName());
    }

    private void SetUpLayout() {
        Container content = this.getContentPane();
        content.add(this.mapForm, BorderLayout.CENTER);
        JPanel buttonPanel = new JPanel();
        buttonPanel.setBackground(Color.lightGray);
        var buttonStart = new JButton(ButtonStartText);
        var buttonStop = new JButton(ButtonStopText);
        buttonStart.addActionListener(e -> OnButtonStart());
        buttonStop.addActionListener(e -> OnButtonStop());
        buttonPanel.add(buttonStart);
        buttonPanel.add(buttonStop);
        content.add(buttonPanel, BorderLayout.SOUTH);
    }

    private void OnButtonStart() {
        var ball = new Ball(mapForm.map);
        BallThread thread = new BallThread(ball);
        thread.start();
    }

    private void OnButtonStop() {
        System.exit(0);
    }
}