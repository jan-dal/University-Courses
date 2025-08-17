import javax.swing.*;

public class GameEndWindow {

    public GameEndWindow(int type){
        String msg = null;

        switch(type){
            case 0 : msg = "Checkmate!";
            break;
            case 1 : msg = "Timeout";
            break;
            case 2 : msg = "Stalemate";
            break;
            case 3 : msg = "Draw";
            break;
        }


        // Create the main Frame
        JFrame root = new JFrame("Game End");

        root.setLocationRelativeTo(null);

        JOptionPane.showMessageDialog(root, msg);

        root.dispose();
    }
}