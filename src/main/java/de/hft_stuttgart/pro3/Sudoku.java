package de.hft_stuttgart.pro3;

import java.awt.EventQueue;

public class Sudoku {
    public static void main(String[] args) {
        EventQueue.invokeLater(()->{
            MainFrame mainFrame = new MainFrame();
            mainFrame.setVisible(true);
        });
    }
}
