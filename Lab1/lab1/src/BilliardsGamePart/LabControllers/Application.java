package BilliardsGamePart.LabControllers;

import BilliardsGamePart.LabThreads.BallThreadFactory;
import BilliardsGamePart.Models.*;

import javax.swing.*;
import java.awt.*;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

public class Application extends JFrame implements PropertyChangeListener {

    private static final String TITLE = "BilliardsGame.Bounce program";
    private static final String STOP_TEXT = "Stop";
    private static final String RED_TEXT = "Red";
    private static final String BLUE_TEXT = "Blue";
    private static final String BOTH_TEXT = "Both";
    private static final String RANDOM_TEXT = "Random";
    private static final String ENDED_THREADS_TEXT = "Ended threads: ";
    private static final String ENDED_THREADS_COUNTER_TEXT = "0";
    private static final String IN_FRAME_THREAD_NAME = "In Frame thread name = ";

    private static final int HOLES_COUNT = 4;
    private static final int BLUE_RED_BOTH_DIFF = 10;

    private static final double MIN_VELOCITY_PERCENTAGE = 0.01;
    private static final double MAX_VELOCITY_PERCENTAGE = 0.04;
    private static final double RADIUS_PERCENTAGE = 0.05;

    private static final int BUTTON_PANEL_HEIGHT = 76;

    private final MapObjectFactory mapObjectFactory = new MapObjectFactory();
    private final BallThreadFactory ballThreadFactory = new BallThreadFactory();
    private final BoardForm boardForm = new BoardForm();
    private final JCheckBox randomCheckBox = new JCheckBox(RANDOM_TEXT);
    private final JLabel endedThreadsLabel = new JLabel(ENDED_THREADS_TEXT);
    private final JLabel joinedThreadsCounterLabel = new JLabel(ENDED_THREADS_COUNTER_TEXT);
    private final JPanel buttonPanel = new JPanel();

    private Integer joinedThreadsCounter = 0;

    private static Application instance;

    private Application() {}

    public static Application getInstance() {
        if(instance == null) {
            instance = new Application();
        }
        return instance;
    }

    public void init(int width, int height) {
        initApplicationForm(width, height);
        initLayout();
        initMainMap();
        initMapObjectFactory();
        initHoles();
        System.out.println(IN_FRAME_THREAD_NAME + Thread.currentThread().getName());
    }

    void initApplicationForm(int width, int height) {
        setSize(width, height);
        setTitle(TITLE);
    }

    private void initLayout() {
        Container content = getContentPane();
        boardForm.setBackground(Color.green);
        content.add(boardForm, BorderLayout.CENTER);

        buttonPanel.setBackground(Color.lightGray);

        var buttonStop = new JButton(STOP_TEXT);
        var buttonBallRed = new JButton(RED_TEXT);
        buttonBallRed.setBackground(Color.red);
        var buttonBallBlue = new JButton(BLUE_TEXT);
        buttonBallBlue.setBackground(Color.blue);
        var buttonBallBoth = new JButton(BOTH_TEXT);

        buttonStop.addActionListener(e -> OnButtonStop());
        buttonBallRed.addActionListener(e -> OnButtonBallRed());
        buttonBallBlue.addActionListener(e -> OnButtonBallBlue());
        buttonBallBoth.addActionListener(e -> OnButtonBallBoth());

        buttonPanel.add(buttonStop);
        buttonPanel.add(buttonBallRed);
        buttonPanel.add(buttonBallBlue);
        buttonPanel.add(buttonBallBoth);
        buttonPanel.add(randomCheckBox);
        buttonPanel.add(endedThreadsLabel);
        buttonPanel.add(joinedThreadsCounterLabel);

        buttonPanel.setSize(getWidth(), BUTTON_PANEL_HEIGHT);

        content.add(buttonPanel, BorderLayout.SOUTH);
    }

    private void initMainMap() {
        var mainMap = MainMap.getInstance();
        mainMap.setSize(getWidth(), getHeight() - BUTTON_PANEL_HEIGHT);
    }

    private void initHoles() {
        var mainMap = MainMap.getInstance();
        var holes = new MapObject[HOLES_COUNT];
        for(var i = 0; i < HOLES_COUNT; ++i) {
            holes[i] = mapObjectFactory.createHole();
        }
        var diam = holes[0].getDiam();
        var size = mainMap.getSize();
        var maxX = size.getX() - diam;
        var maxY = size.getY() - diam;
        holes[0].setLocation(0, 0);
        holes[1].setLocation(maxX, 0);
        holes[2].setLocation(0, maxY);
        holes[3].setLocation(maxX, maxY);
    }

    private void initMapObjectFactory() {
        var params = new MapObjectFactory.Params();
        params.minVelocityPercentage = MIN_VELOCITY_PERCENTAGE;
        params.maxVelocityPercentage = MAX_VELOCITY_PERCENTAGE;
        params.radiusPercentage = RADIUS_PERCENTAGE;
        var size = MainMap.getInstance().getSize();
        params.straightLocation.set(1, size.getY() / 2);
        params.straightVelocity.set(size.getX() * MIN_VELOCITY_PERCENTAGE, 0);
        mapObjectFactory.setParams(params);
    }

    private ObjectBehaviour getCreationType() {
        if(randomCheckBox.isSelected()) {
            return ObjectBehaviour.Randomized;
        }
        return ObjectBehaviour.Straight;
    }

    private void createThread(ObjectColor color) {
        var creationType = getCreationType();
        var ball = mapObjectFactory
                .createBall(color, creationType);
        var t = ballThreadFactory.createBallThread(ball);
        t.start();
    }

    private void createRedThread() {
        createThread(ObjectColor.Red);
    }

    private void createBlueThread() {
        createThread(ObjectColor.Blue);
    }

    private void OnButtonStop() {
        System.exit(0);
    }

    private void OnButtonBallRed() {
        createRedThread();
    }

    private void OnButtonBallBlue() {
        createBlueThread();
    }

    private void OnButtonBallBoth() {
        createRedThread();
        for(var i = 0; i < BLUE_RED_BOTH_DIFF; ++i) {
            createBlueThread();
        }
    }

    private synchronized void incrementJoinedThreadsLabel() {
        joinedThreadsCounter++;
        joinedThreadsCounterLabel.setText(joinedThreadsCounter.toString());
    }

    @Override
    public void propertyChange(PropertyChangeEvent evt) {
        incrementJoinedThreadsLabel();
    }
}