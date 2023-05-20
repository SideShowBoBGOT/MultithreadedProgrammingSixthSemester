package LabForms;

import LabLogic.BallMap;
import LabVisuals.BallDrawer;

import javax.swing.*;
import java.awt.*;

public class MapForm extends JPanel {
    public MapForm() {}

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        for(LabLogic.Ball b : BallMap.getInstance().getBalls()) {
            BallDrawer.draw(g2, b);
        }
        repaint();
    }
}
