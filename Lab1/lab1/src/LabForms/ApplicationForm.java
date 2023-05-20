package LabForms;

import LabLogic.BallFactory;
import LabLogic.BallMap;
import LabThreads.BallThread;

import javax.swing.*;
import java.awt.*;

public class ApplicationForm extends JFrame {

    private static final String TITLE = "Bounce program";
    private static final String BUTTON_START_TEXT = "Start";
    private static final String BUTTON_STOP_TEXT = "Stop";
    private static final String IN_FRAME_THREAD_NAME = "In Frame thread name = ";

    private static final double MIN_VELOCITY_PERCENTAGE = 0.01;
    private static final double MAX_VELOCITY_PERCENTAGE = 0.04;
    private static final double RADIUS_PERCENTAGE = 0.05;
    private static final double RED_COLOR_PERCENTAGE = 0.3;
    private static final double BLUE_COLOR_PERCENTAGE = 0.5;
    private static final double GREEN_COLOR_PERCENTAGE = 0.2;

    private final BallFactory ballFactory = new BallFactory();
    private final MapForm mapForm;

    public ApplicationForm(int width, int height) {
        setSize(width, height);
        setTitle(TITLE);
        BallMap.getInstance().setSize(width, height);
        mapForm = new MapForm();
        initLayout();
        initBallFactory();
        System.out.println(IN_FRAME_THREAD_NAME + Thread.currentThread().getName());
    }

    private void initLayout() {
        Container content = this.getContentPane();
        content.add(mapForm, BorderLayout.CENTER);
        JPanel buttonPanel = new JPanel();
        buttonPanel.setBackground(Color.lightGray);
        var buttonStart = new JButton(BUTTON_START_TEXT);
        var buttonStop = new JButton(BUTTON_STOP_TEXT);
        buttonStart.addActionListener(e -> OnButtonStart());
        buttonStop.addActionListener(e -> OnButtonStop());
        buttonPanel.add(buttonStart);
        buttonPanel.add(buttonStop);
        content.add(buttonPanel, BorderLayout.SOUTH);
    }

    private void initBallFactory() {
        var params = new BallFactory.BallFactoryPrams();
        params.minVelocityPercentage = MIN_VELOCITY_PERCENTAGE;
        params.maxVelocityPercentage = MAX_VELOCITY_PERCENTAGE;
        params.radiusPercentage = RADIUS_PERCENTAGE;
        params.redColorPercentage = RED_COLOR_PERCENTAGE;
        params.blueColorPercentage = BLUE_COLOR_PERCENTAGE;
        params.greenColorPercentage = GREEN_COLOR_PERCENTAGE;
        ballFactory.setParams(params);
    }

    private void OnButtonStart() {
        var ball = ballFactory.createBall();
        BallThread thread = new BallThread(ball);
        thread.start();
    }

    private void OnButtonStop() {
        System.exit(0);
    }
}