package de.hft_stuttgart.pro3;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.KeyboardFocusManager;
import java.awt.MouseInfo;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Deque;
import java.util.List;
import java.util.stream.IntStream;
import javax.swing.*;


/**
 *
 * @author Peter Heusch
 */
public class SudokuPanel extends JPanel {
    private final static int SIZEL = 50;
    private final static int GRIDL = SIZEL + SIZEL / 10;
    private final static int GRIDS = GRIDL / 10;
    private StringBuilder [] rowBuffers;
    private Character [] textBuffer;
    private String [] hintBuffer;
    private Color[] textForeground;
    private Color[] textBackground;
    private Dimension shift;

    private KeyListener keyListener;
    private Window myParent;
    private String toolTip;
    private int currentPos;

    private int dumps;

    public SudokuPanel(Window parent) {
        dumps = 0;
        myParent = parent;

        textBuffer = new Character[81];
        hintBuffer = new String[81];
        textForeground = new Color[81];
        textBackground = new Color[81];

        for (int i = 0; i < textBuffer.length; i++) {
            textBuffer[i] = ' ';
            textForeground[i] = Color.black;
            textBackground[i] = new Color(192, 192, 192);
        }

        setMaximumSize(new java.awt.Dimension(500, 500));
        setMinimumSize(new java.awt.Dimension(500, 500));
        setPreferredSize(new java.awt.Dimension(500, 500));
        updateTooltips();

        KeyboardFocusManager kfm = KeyboardFocusManager.getCurrentKeyboardFocusManager();

        kfm.addKeyEventDispatcher(e -> {
            KeyStroke ks = KeyStroke.getKeyStrokeForEvent(e);
            if ( ks.equals(KeyStroke.getAWTKeyStroke("control alt pressed Q")) ) {
                JOptionPane.showMessageDialog(SudokuPanel.this, "Und Tschüss", "Meldung", 
                        JOptionPane.INFORMATION_MESSAGE);
                myParent.setVisible(false);
                myParent.dispose();
                return true;
            }
            return false;
        });

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                currentPos = e.getY()/GRIDL * 9 + e.getX() / GRIDL;
                System.out.printf("At Pos %d%n", currentPos);
            }
        });

        addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                super.keyTyped(e);
                char c = e.getKeyChar();
                switch(c) {
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        textBuffer[currentPos] = c;
                        checkPlausibility(currentPos, c);
                        updateTooltips();
                        repaint();
                        break;
                    default:
                        textBuffer[currentPos] = ' ';
                        checkPlausibility(currentPos, c);
                        updateTooltips();
                        repaint();
                        break;
                }
            }
        });
    }

        @Override
        public void paintComponent(Graphics gc) {
            super.paintComponent(gc);
            Graphics2D g2d = (Graphics2D) gc;
            Font large = new Font("Helvetica", Font.PLAIN, 3*SIZEL/5);
            Font small = new Font("Helvetica", Font.PLAIN, 3*SIZEL/10);

            shift = getSize();
            shift.width = shift.width / 2 - 9*GRIDL/2;
            shift.height = shift.height / 2 - 9*GRIDL/2;
            System.out.printf("Shift: %d %d%n", shift.width, shift.height);
            g2d.translate(shift.width, shift.height);

            g2d.setStroke(new BasicStroke(2.0f));
            g2d.setColor(new Color(192, 192, 192));
            //g2d.fillRect(-2, -2, GRID*2 + SIZE + 4, GRID*2 + SIZE + 4);
            //g2d.fillRect(-2, -2 + GRID*5 + SIZE + 4, GRID*2 + SIZE +4, GRID*2 + SIZE + 4);
            //g2d.fillRect(-2 + GRID*5 + SIZE + 4, -2, GRID*2 + SIZE + 4, GRID*2 + SIZE + 4);
            //g2d.fillRect(-2 + GRID*5 + SIZE + 4, -2 + GRID*5 + SIZE + 4, GRID*2 + SIZE +4, GRID*2 + SIZE + 4);
            //g2d.fillRect(-2 + GRID*2 + SIZE + 4, -2 + GRID*2 + SIZE + 4, GRID*2 + SIZE +4, GRID*2 + SIZE + 4);

            for (int i = 0; i < 81; i++) {
                int row = i / 9;
                int col = i % 9;
                int blk = i/27*3 + i%9/3;
                g2d.setStroke(new BasicStroke(2.0f));
                g2d.setColor(new Color(64, 64, 64));
                g2d.fillRect(GRIDL*col, GRIDL*row, SIZEL, SIZEL);
                g2d.setColor(textBackground[i]);
                g2d.fillRect(GRIDL*col+SIZEL/10, GRIDL*row+SIZEL/10, 8*SIZEL/10, 8*SIZEL/10);
                g2d.setColor(textForeground[i]);
                if ( textBuffer[i] == ' ') {
                    g2d.setFont(small);
                    g2d.setColor(Color.yellow);
                    String hint = hintBuffer[i].concat("         ");
                    g2d.drawString(hint.substring(0, 3), GRIDL*col+SIZEL/10+5, GRIDL*row+SIZEL/3+3);
                    g2d.drawString(hint.substring(3, 6), GRIDL*col+SIZEL/10+5, GRIDL*row+SIZEL/3+15);
                    g2d.drawString(hint.substring(6, 9), GRIDL*col+SIZEL/10+5, GRIDL*row+SIZEL/3+27);
                }
                else {
                    g2d.setFont(large);
                    FontMetrics fm = g2d.getFontMetrics();
                    int width = fm.stringWidth(textBuffer[i].toString());
                    int height = fm.getHeight();
                    g2d.drawString(textBuffer[i].toString(),
                            GRIDL*col+5*SIZEL/10-width/2, GRIDL*row+5*SIZEL/10+height/4);
                }
            }

            requestFocus();
        }

        public boolean isPossible(int pos, char c) {
            int row = pos / 9;
            int col = pos % 9;
            int block = (pos/27)*3 + (pos%9)/3;
            for ( int i=0; i<textBuffer.length; i++ ) {
                if ( i == pos ) {
                    continue;
                }
                if ( textBuffer[i] != c ) {
                    continue;
                }
                if ( i / 9 == row ||
                        i % 9 == col ||
                        (i/27)*3 + (i%9)/3 == block) {
                    return false;
                }
            }
            return true;
        }

        private boolean isUnique(int pos, char key) {
            if ( textBuffer[pos]!=' ' ) {
                return false;
            }
            int row = pos / 9;
            int col = pos % 9;
            int blk = (pos/27)*3 + (pos%9)/3;
            boolean uniqueRow = true;
            boolean uniqueColumn = true;
            boolean uniqueBlock = true;

            for ( int i=0; i<81; i++) {
                if ( i == pos || textBuffer[i] != ' ' ) {
                    continue;
                }
                int irow = i/9;
                int icol = i%9;
                int iblk = (i/27)*3 + (i%9)/3;
                if ( irow != row && icol != col && iblk != blk) {
                    continue;
                }

                if ( textBuffer[i] != ' ' || !isPossible(i, key) ) {
                    continue;
                }

                if ( irow == row ) {
                    uniqueRow = false;
                }
                if ( icol == col ) {
                    uniqueColumn = false;
                }
                if ( iblk == blk ) {
                    uniqueBlock = false;
                }
            }
            return uniqueRow || uniqueColumn || uniqueBlock;
        }

        public void keyTyped(KeyEvent e) {
            int pos = getMousePos();
            if ( pos < 0 || pos >= 81 ) {
                return;
            }
            System.out.printf("At pos %d %n", pos);
            char keyPressed = e.getKeyChar();
            e.consume();
            if ( keyPressed < '0' || keyPressed > '9' ){
                textBuffer[pos] = ' ';
            }
            else {
                textBuffer[pos] = keyPressed;
            }
            checkPlausibility(pos, keyPressed);
            updateTooltips();
            repaint();
        }

        private int getMousePos() {
            Point point = MouseInfo.getPointerInfo().getLocation();
            Rectangle rect = getBounds();
            point.x = point.x-shift.width-rect.x;
            point.y = point.y-shift.height-rect.y;
            int row = point.y / GRIDL;
            int col = point.x / GRIDL;
            return row * 9 + col;
        }

        private void checkPlausibility(int pos, char keyPressed) {
            textForeground[pos] = isPossible(pos, keyPressed)?
                    Color.BLACK:Color.RED;
            for (int i = 0; i < 81; i++) {
                if ( textBuffer[i] == ' ' ) {
                    continue;
                }
                textForeground[i] = isPossible(i, textBuffer[i])?
                        Color.BLACK:Color.RED;
            }
        }

        private void updateTooltips() {
            for (int i = 0; i < 81; i++) {
                if ( textBuffer[i] != ' ' ) {
                    textBackground[i] = new Color(192,192,192);
                    hintBuffer[i] = "";
                    //setToolTipText(null);
                    continue;
                }
                int count = 0;
                boolean unique = false;
                StringBuilder choices = new StringBuilder();
                for ( char c: "123456789".toCharArray() ) {
                    if ( isPossible(i, c) ) {
                        count++;
                        if ( isUnique(i, c) ) {
                            choices.append("<" + c + ">");
                            unique = true;
                        }
                        else {
                            choices.append(c);
                        }
                    }
                }
                hintBuffer[i] = choices.toString();
                if ( count == 0) {
                    textBackground[i] = Color.yellow;
                }
                else if ( unique ) {
                    textBackground[i] = Color.green;
                }
                else if ( count == 1 ){
                    textBackground[i] = Color.green;
                }
                else {
                    textBackground[i] = new Color(192,192,192);
                }
            }
        }

    public void createSolution() {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                textBuffer[9*row+col] = ' ';
            }
        }

        initialize(0, 3, 0, 3);
        initialize(3, 6, 3, 6);
        initialize(6, 9, 6, 9);
        // Erste passende Loesung finden
        complete(1);
        updateTooltips();
        // Hier noch einbauen: Solange zufaellig Zahlen loeschen (d.h. durch ' ' ersetzen), bis
        // complete(2) funktioniert und die letzte geloeschte Zahl dann wieder einsetzen und bis
        // die max. Zahl zu loeschender Zahlen erreicht ist.
        repaint();
    }

    private void initialize(int firstRow, int lastRow, int firstCol, int lastCol) {
        List<Integer> firstBlock = new ArrayList(IntStream.rangeClosed(1, 9).boxed().toList());
        Collections.shuffle(firstBlock);
        for (int row = firstRow; row < lastRow; row++) {
            for (int col = firstCol; col < lastCol; col++) {
                textBuffer[9*row+col] = (char)(firstBlock.get(0).intValue()+'0');
                firstBlock.remove(0);
            }
        }
    }

    private record Attempt(int pos, char guess) {
        private Attempt next() {
            return guess == '9' ? null : new Attempt(pos, (char)(guess+1));
        }
    }
    private int complete(int maxCount) {
        int found = 0;
        Deque<Attempt> attempts = new ArrayDeque<>();
        for (int index = 0; index < 81; index++) {
            if ( textBuffer[index] == ' ') {
                attempts.addLast(new Attempt(index, '1'));
                textBuffer[index] = '1';
                break;
            }
        }
        while ( !attempts.isEmpty() ) {
            dumpData();
            Attempt current = attempts.peekLast();
            if ( isPossible(current.pos, current.guess) ) {
                int nextFree = findFree();
                if ( nextFree == 81 ) {
                    if ( ++found == maxCount ) {
                        updateTooltips();
                        repaint();
                        return found;
                    }
                    Attempt next = current.next();
                    while ( next == null ) {
                        attempts.removeLast();
                        textBuffer[current.pos] = ' ';
                        current = attempts.peekLast();
                        attempts.removeLast();
                        next = current.next();
                    }
                    attempts.removeLast();
                    attempts.addLast(next);
                    textBuffer[next.pos] = next.guess;
                }
                else {
                    attempts.addLast(new Attempt(nextFree, '1'));
                    textBuffer[nextFree] = '1';
                }
            }
            else {
                Attempt next = current.next();
                while ( next == null ) {
                    attempts.removeLast();
                    textBuffer[current.pos] = ' ';
                    current = attempts.peekLast();
                    next = current.next();
                }
                if (next != null) {
                    attempts.removeLast();
                    attempts.addLast(next);
                    textBuffer[next.pos] = next.guess;
                }
            }
        }
        return -1;
    }

    private void dumpData() {
        System.out.printf("Dump %d%n", ++dumps);
        System.out.println("-------------");
        System.out.print('|');
        for (int i = 0; i < 81; i++) {
            System.out.print(textBuffer[i]);
            if ((i % 3) == 2) {
                System.out.print('|');
            }
            if ((i % 9) == 8) {
                System.out.println();
                if ( i%27 == 26 ) {
                    System.out.println("-------------");
                }
                System.out.print('|');
            }
        }
        System.out.println();
    }

    private int findFree() {
        for (int i = 0; i < 81; i++) {
            if ( textBuffer[i] == ' ') {
                return i;
            }
        }
        return 81;
    }
}
