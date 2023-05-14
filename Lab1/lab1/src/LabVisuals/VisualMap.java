package LabVisuals;

import LabLogic.Map;

import javax.swing.*;
import java.awt.*;

public class VisualMap extends JPanel {
    private final LabLogic.Map map;
    public VisualMap(LabLogic.Map inMap) {
        map = inMap;
    }
    public Map GetMap() { return map.clone(); }
    @Override
    public void paintComponent(Graphics g){
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D)g;
        for(Ballwww b : ) {
            b.Draw(g2);
        }
    }
}
