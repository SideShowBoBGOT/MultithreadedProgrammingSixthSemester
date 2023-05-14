package LabForms;

import LabVisuals.BallDrawer;

import javax.swing.*;
import java.awt.*;

public class MapForm extends JPanel {
    private final LabLogic.Map map;
    public MapForm(LabLogic.Map inMap) {
        map = inMap;
    }
    @Override
    public void paintComponent(Graphics g){
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D)g;
        for(LabLogic.Ball b : map.GetBalls()) {
            BallDrawer.Draw(g2, b);
        }
        repaint();;
    }
}
