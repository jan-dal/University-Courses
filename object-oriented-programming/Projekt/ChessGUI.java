import java.awt.event.*;

import javax.swing.*;
import java.awt.*;

class ChessGUI{ //ItemListener
    ChessBoardGUI chessboard;
    InformationGUI infobox;

    public void setupGUI(ActionListener a){
        // Create the main Frame
        JFrame root = new JFrame("Chess");
        root.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        root.setLocationRelativeTo(null);

        // Create menu
        JMenuBar menuBar = new JMenuBar();
        menuBar.setOpaque(true);

        JMenu menu = new JMenu("Game");
        
        // Start Game
        JMenuItem menuItem = new JMenuItem("Start game");
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_G,ActionEvent.CTRL_MASK));
        menuItem.addActionListener(a);
        menu.add(menuItem);

        // Stop Game
        menuItem = new JMenuItem("Stop game");
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X,ActionEvent.CTRL_MASK));
        menuItem.addActionListener(a);
        menu.add(menuItem);

        // Save Option
        menuItem = new JMenuItem("Save");
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S,ActionEvent.CTRL_MASK));
        menuItem.addActionListener(a);
        menu.add(menuItem);

        // Load option
        menuItem = new JMenuItem("Load");
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_L,ActionEvent.CTRL_MASK));
        menuItem.addActionListener(a);
        menu.add(menuItem);

        // Reload Board
        menuItem = new JMenuItem("Reload board");
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_R,ActionEvent.CTRL_MASK));
        menuItem.addActionListener(a);
        menu.add(menuItem);

        //Flip board
        menuItem = new JMenuItem("Flip board");
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F,ActionEvent.CTRL_MASK));
        menuItem.addActionListener(a);
        menu.add(menuItem);


        JPanel container = new JPanel();
        container.setLayout(new BoxLayout(container, BoxLayout.X_AXIS));

        chessboard = new ChessBoardGUI();
        infobox = new InformationGUI();

        container.setBackground(new Color(49,46,43));
        container.add(chessboard);
        container.add(infobox);


        root.setContentPane(container);

        // Add menu to menuBar
        menuBar.add(menu);
        root.setJMenuBar(menuBar);

        // Set visible to complete the setup
        root.pack();
        root.setVisible(true);
    }


    public ChessBoardGUI getChessboard(){
        return chessboard;
    }
    public InformationGUI getInfobox(){
        return infobox;
    }
}