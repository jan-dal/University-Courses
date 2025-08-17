import javax.swing.*;

public class ChoosePieceWindow {
    JTextField text;
    JFrame root;
    int pos;

    public void CreatePiece(String x){
        Piece p;
        String Side;

        if(Character.isUpperCase(x.charAt(0))){
            Side = "w";
        } else {
            Side = "b";
        }
        p = BoardData.CreatePiece(Character.toString(Character.toUpperCase(x.charAt(0))), Side, pos);
        if(p != null){
            BoardData.Add(p);
            BoardData.Remove(BoardData.get(pos).getPiece());
            BoardData.get(pos).SetPiece(p);
        }
    }

    public ChoosePieceWindow(int pos){
        this.pos = pos;
        // Create the main Frame
        root = new JFrame("Create new piece");

        root.setLocationRelativeTo(null);

        String s = (String)JOptionPane.showInputDialog(
            root, "",
            "Choose a piece",
            JOptionPane.PLAIN_MESSAGE,
            new ImageIcon(),
            null,
            "Q");
        if(s != null){
            CreatePiece(s);
        }
        root.dispose();
    }

}