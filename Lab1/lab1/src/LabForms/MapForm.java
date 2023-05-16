package LabForms;

import LabLogic.Ball;
import LabVisuals.BallDrawer;

import javax.swing.*;
import java.awt.*;

public class MapForm extends JPanel {
    public final LabLogic.Map map;

    public MapForm(LabLogic.Map inMap) {
        map = inMap;
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        for(LabLogic.Ball b : map.getBalls()) {
            BallDrawer.draw(g2, b);
        }
        repaint();
    }
}
