import javax.swing.*;
import java.awt.*;
// P - Pawn
// B - Bishop
// N - Knight
// R - Rook
// Q - Queen
// K - King


public class PieceGUI extends JLabel{
    Piece piece;
    Piece sim_piece;
    boolean visible;


    public PieceGUI(){
    // For a future piece
        piece = null;
        visible = true;
        sim_piece = null;
        this.setOpaque(false);
        this.setHorizontalAlignment(JLabel.CENTER);
        this.setVerticalAlignment(JLabel.CENTER);
    }


    public void SetPiece(Piece piece){
        this.piece = piece;
        this.setIcon(new ImageIcon("img/Chess_" + piece.getType() + piece.getSide() + ".png"));
        repaint();
    }

    public void SetSimPiece(Piece piece){
        sim_piece = piece;
    }

    public void RemoveSimPiece(){
        sim_piece = null;
    }

    public Piece getSimPiece(){
        return sim_piece;
    }

    public Piece getPiece(){
        return piece;
    }

    public boolean getVisible(){
        return visible;
    }

    public void setVisible(boolean v){
        visible = v;
    }

    public boolean hasSimPiece(){
        return sim_piece != null;
    }

    public void RemovePiece(){
        this.setIcon(null);
        piece = null;
        repaint();
    }

    public boolean emptySquare(){
        return piece == null;
    }

    public void AttackedPiece(){
        this.setOpaque(true);
        setBackground(new Color(188,75,81)); //Red
        repaint();
    }
    public void MovingPossible(){
        this.setOpaque(true);
        setBackground(new Color(255,255,153)); //Yellow
        repaint();
    }

    public void SourceSquare(){
        this.setOpaque(true);
        setBackground(new Color(255,255,112)); //Faint 
        repaint();
    }

    public void CurrentSquare(){
        this.setOpaque(true);
        setBackground(new Color(255,255,51)); // Strong yellow
        repaint();
    }

    public void NeutralSquare(){
        this.setOpaque(false);
        repaint();
    }

}