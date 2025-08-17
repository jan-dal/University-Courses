import javax.swing.*;

public class LoadGameWindow {
    JTextField text;
    JFrame root;
    int pos;

    public void LoadGame(String x){
        BoardData.loadBoard(x);
    }

    public LoadGameWindow(){
        // Create the main Frame
        root = new JFrame("Create new piece");

        root.setLocationRelativeTo(null);

        String s = (String)JOptionPane.showInputDialog(
            root, "",
            "Paste a fen position",
            JOptionPane.PLAIN_MESSAGE,
            new ImageIcon(),
            null,
            "");
        if(s != null){
            LoadGame(s);
        }
        root.dispose();
    }

}
