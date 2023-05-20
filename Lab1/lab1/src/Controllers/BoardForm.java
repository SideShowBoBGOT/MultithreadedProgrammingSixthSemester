package Controllers;

import Models.MainMap;
import LabVisuals.MapObjectDrawer;
import Models.MapObject;

import javax.swing.*;
import java.awt.*;

public class BoardForm extends JPanel {
    public BoardForm() {}

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        for(MapObject b : MainMap.getInstance().getBalls()) {
            MapObjectDrawer.draw(g2, b);
        }
        repaint();
    }
}
