package Controllers;

import Models.MapObjectFactory;
import Models.MainMap;
import LabThreads.BallThread;
import Models.MapObject;
import Models.ObjectType;

import javax.swing.*;
import java.awt.*;

public class ApplicationForm extends JFrame {

    private static final String TITLE = "Bounce program";
    private static final String START_TEXT = "Start";
    private static final String STOP_TEXT = "Stop";
    private static final String RED_TEXT = "Red";
    private static final String BLUE_TEXT = "Blue";
    private static final String RANDOM_TEXT = "Random";
    private static final String ENDED_THREADS_TEXT = "Ended threads: ";
    private static final String ENDED_THREADS_COUNTER_TEXT = "0";

    private static final int HOLES_COUNT = 4;

    private static final String IN_FRAME_THREAD_NAME = "In Frame thread name = ";

    private static final double MIN_VELOCITY_PERCENTAGE = 0.01;
    private static final double MAX_VELOCITY_PERCENTAGE = 0.04;
    private static final double RADIUS_PERCENTAGE = 0.05;

    private final MapObjectFactory ballFactory = new MapObjectFactory();
    private final BoardForm boardForm = new BoardForm();
    private final JCheckBox randomCheckBox = new JCheckBox(RANDOM_TEXT);
    private final JLabel endedThreadsLabel = new JLabel(ENDED_THREADS_TEXT);
    private final JLabel endedThreadsCounter = new JLabel(ENDED_THREADS_COUNTER_TEXT);

    public ApplicationForm(int width, int height) {
        initApplicationForm(width, height);
        initMainMap();
        initLayout();
        initBallFactory();
        System.out.println(IN_FRAME_THREAD_NAME + Thread.currentThread().getName());
    }

    void initApplicationForm(int width, int height) {
        setSize(width, height);
        setTitle(TITLE);
    }

    void initMainMap() {
        var mainMap = MainMap.getInstance();
        mainMap.setSize(getWidth(), getHeight());
        var holes = new MapObject[HOLES_COUNT];
        for(var i = 0; i < HOLES_COUNT; ++i) {
            holes[i] = new MapObject();
            holes[i].setType(ObjectType.Hole);
            holes[i].setRadius();
            mainMap.addMapObject(holes[i]);
        }
    }

    private void initLayout() {
        Container content = this.getContentPane();
        content.add(boardForm, BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setBackground(Color.lightGray);

        var buttonStart = new JButton(START_TEXT);
        var buttonStop = new JButton(STOP_TEXT);
        var buttonBallRed = new JButton(RED_TEXT);
        buttonBallRed.setBackground(Color.red);
        var buttonBallBlue = new JButton(BLUE_TEXT);
        buttonBallBlue.setBackground(Color.blue);

        buttonStart.addActionListener(e -> OnButtonStart());
        buttonStop.addActionListener(e -> OnButtonStop());
        buttonStop.addActionListener(e -> OnButtonBallRed());
        buttonStop.addActionListener(e -> OnButtonBallBlue());

        buttonPanel.add(buttonStart);
        buttonPanel.add(buttonStop);
        buttonPanel.add(buttonBallRed);
        buttonPanel.add(buttonBallBlue);
        buttonPanel.add(randomCheckBox);
        buttonPanel.add(endedThreadsLabel);
        buttonPanel.add(endedThreadsCounter);

        content.add(buttonPanel, BorderLayout.SOUTH);
    }

    private void initBallFactory() {
        var params = new MapObjectFactory.Params();
        params.minVelocityPercentage = MIN_VELOCITY_PERCENTAGE;
        params.maxVelocityPercentage = MAX_VELOCITY_PERCENTAGE;
        params.radiusPercentage = RADIUS_PERCENTAGE;
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
    private void OnButtonBallRed() {}
    private void OnButtonBallBlue() {}
}