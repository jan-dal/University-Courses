import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseAdapter;

public abstract class MovementControl extends MouseAdapter{
    protected ImageGUI grid;
    protected Point click, loc;
    protected int zorder;

    // What happens when a piece is placed
    protected abstract void placePiece(SquareGUI square, MovPair dest);
    protected void movePiece(SquareGUI square, MouseEvent e){
        // Piece move piece to location if legal && ingame else set back
        // Update BoardData 
        int thisX = square.getLocation().x;
        int thisY = square.getLocation().y;
    
        int xMoved = (thisX + e.getX()) - (thisX + click.x);
        int yMoved = (thisY + e.getY()) - (thisY + click.y);
    
        int X = thisX + xMoved;
        int Y = thisY + yMoved; 
        square.setLocation(X, Y);
    }

    protected int getcell(Point pos){

        if (pos != null){
            int gridX = grid.getWidth();
            int gridY = grid.getHeight();
            int x = (int)pos.getX();
            int y = (int)pos.getY();
    
            int mx = (int)(x*8 / gridX);
            int my = (int)(y*8 / gridY);
    
            return my*8 + mx;
        } else {
            return 0;
        }
    }

    public void setGrid(ImageGUI x){
        grid = x;
    }
}

class GameMovementControl extends MovementControl{
    String tomove;
    TimerGUI timer1, timer2;
    ActionListener listener;

    public void setActionListener(ActionListener l){
        listener = l;
    }

    public boolean IsCheckmate(){
        if(tomove.equals("w")){
            for(Piece e : BoardData.getWhite()){
                if(e.getMovelist() != null && e.getMovelist().size() > 0){
                    return false;
                }
            }
        } else {
            for(Piece e : BoardData.getBlack()){
                if(e.getMovelist() != null && e.getMovelist().size() > 0){
                    return false;
                }
            }
        }
        return true;
    }

    public void CalculateAllMoves(){
        if(BoardData.get(BoardData.getKing(tomove).getPosition()).isAttacked(tomove, BoardData.getKing(tomove).getPosition())){
            BoardData.get(BoardData.getKing(tomove).getPosition()).AttackedPiece();
            ((King)(BoardData.getKing(tomove))).setInCheck(true);

        }

        if(tomove.equals("w")){
            for(Piece e : BoardData.getWhite()){
                if(e.getActive()){
                    e.updateMovelist();
                }
            }
            for(Piece e : BoardData.getBlack()){
                e.setMovelist(null);
            }
        } else {
            for(Piece e : BoardData.getBlack()){
                if(e.getActive()){
                    e.updateMovelist();
                }
            }
            for(Piece e : BoardData.getWhite()){
                e.setMovelist(null);
            }
        }

        if(IsCheckmate()){
            if(((King)(BoardData.getKing(tomove))).inCheck()){
                //Checkmate
                listener.actionPerformed(new ActionEvent(this, 0, "Stop game"));
                new GameEndWindow(0);
            } else {
                //Stalemate
                listener.actionPerformed(new ActionEvent(this, 0, "Stop game"));
                new GameEndWindow(2);
            }
            
        }
    }

    public void setToMove(String m){
        tomove = m;
    }

    public void nextToMove(){
        if(tomove.equals("w")){
            tomove = "b";
        } else {
            tomove = "w";
        }
    }

    public void setTimers(TimerGUI t1, TimerGUI t2){
        timer1 = t1;
        timer2 = t2;
    }

    @Override
    public void placePiece(SquareGUI square, MovPair d){
        int pos = d.getPos();
        Piece source = square.getPiece();
        SquareGUI dest = BoardData.get(d.getPos());

        BoardData.setMoved(source);
        source.setMoved(true);
        source.setPosition(pos);
        source.addHistory(pos);



        if(!d.getSquare().emptySquare()){
            // Capture Piece
            d.getSquare().getPiece().setActive(false);
            d.getSquare().RemovePiece();

        }

        dest.SetPiece(source);

        square.RemovePiece();
        
        square.setLocation(loc);
        
        if(source instanceof Pawn){
            ((Pawn)(source)).Promotion();
        }

        if(timer1.isRunning()){
            timer1.StopTimer();
            timer2.StartTimer();
        } else {
            timer2.StopTimer();
            timer1.StartTimer();
        }


        KingCheck();
        nextToMove();
        CalculateAllMoves();

    }

    @Override
    public void mousePressed(MouseEvent e){
        // Light up all legal moves for this piece
        SquareGUI square = (SquareGUI)e.getComponent();
        if(square.emptySquare()){
            square.SourceSquare();
        } 
        else {
        // Make the piece stand out
            square.SourceSquare();
            zorder = grid.getComponentZOrder(square);
            grid.setComponentZOrder(square, 0);


        // Gather information about location of the piece
            click = e.getPoint();
            loc = square.getLocation();

        // Lightup all squares 
            ColorSquares(square.getPiece().getMovelist());
        }
    }

    @Override
    public void mouseDragged(MouseEvent e){
        SquareGUI square = (SquareGUI)e.getComponent();
        if(!square.emptySquare()){
            square.NeutralSquare();
            movePiece(square, e);
        }
    }

    @Override
    public void mouseReleased(MouseEvent e){
        SquareGUI square = (SquareGUI)e.getComponent();
        MovPair dest;
        ArrayList<MovPair> movelist;
        square.NeutralSquare();

        if(!square.emptySquare()){
            grid.setComponentZOrder(square, zorder);
            movelist = square.getPiece().getMovelist();
            dest = legal(movelist, getcell(grid.getMousePosition()));
            if(dest != null){
                placePiece(square, dest);
            } else {
                square.setLocation(loc);
            }

            TurnOffSquares(movelist);
        }
    }

    private MovPair legal(ArrayList<MovPair> movlist, int pos){
        if(movlist != null){
            for(MovPair e : movlist){
                if(e.getPos() == pos){
                    return e;
                }
            }
        }
        return null;
    }

    private void TurnOffSquares(ArrayList<MovPair> movelist){
        
        if(movelist != null){
            for(MovPair e : movelist){
                SquareGUI dest = BoardData.get(e.getPos());
                dest.NeutralSquare();
            }
        }
    }

    private void ColorSquares(ArrayList<MovPair> movelist){
        
        if(movelist != null){
            for(MovPair e : movelist){
                SquareGUI pos = e.getSquare();
                SquareGUI dest = BoardData.get(e.getPos());

                if(pos.emptySquare()){
                    dest.MovingPossible();
                }
                else {
                    dest.AttackedPiece();
                }
            }
        }
    }

    private void KingCheck(){
        King k1 = (King)(BoardData.getKing(tomove));
        if(BoardData.getMoved().equals(k1) && k1.Moved()){
            int prevPos = k1.getHistory().get(k1.getHistory().size() - 2);
            if(k1.getPosition() - prevPos == 2){
                if((tomove.equals("w") && !BoardData.isFlipped()) || (tomove.equals("b") && BoardData.isFlipped())){
                    Piece rook = BoardData.get(63).getPiece();
                    rook.setPosition(61);
                    BoardData.get(63).RemovePiece();
                    BoardData.get(61).SetPiece(rook);
                } else {
                    Piece rook = BoardData.get(7).getPiece();
                    rook.setPosition(5);
                    BoardData.get(7).RemovePiece();
                    BoardData.get(5).SetPiece(rook);
                }
            }
            else if(k1.getPosition() - prevPos == -2){
                if((tomove.equals("w") && !BoardData.isFlipped()) || (tomove.equals("b") && BoardData.isFlipped())){
                    Piece rook = BoardData.get(56).getPiece();
                    rook.setPosition(59);
                    BoardData.get(56).RemovePiece();
                    BoardData.get(59).SetPiece(rook);
                } else {
                    Piece rook = BoardData.get(0).getPiece();
                    rook.setPosition(3);
                    BoardData.get(0).RemovePiece();
                    BoardData.get(3).SetPiece(rook);
                }  
            }
        
        }


        
        if(k1.inCheck()){
            k1.setInCheck(false);
        }
        BoardData.get(k1.getPosition()).NeutralSquare();
    }

}

class EditorMovementControl extends MovementControl{

    @Override
    public void placePiece(SquareGUI square, MovPair d){
        int pos = getcell(grid.getMousePosition());
        Piece source = square.getPiece();
        SquareGUI dest = BoardData.get(pos);

        source.setPosition(pos);
        dest.SetPiece(source);
        square.RemovePiece();
        square.setLocation(loc);
    }

    @Override
    public void mousePressed(MouseEvent e){
        // Light up all legal moves for this piece
        SquareGUI square = (SquareGUI)e.getComponent();

        if(SwingUtilities.isRightMouseButton(e)){
            if(square.emptySquare()){
                new ChoosePieceWindow(getcell(grid.getMousePosition()));
            } else {
                BoardData.Remove(square.getPiece());
                square.RemovePiece();
            }
        }
        else {
            square.SourceSquare();
            if(!square.emptySquare()){

                zorder = grid.getComponentZOrder(square);
                grid.setComponentZOrder(square, 0);

            // Gather information about location of the piece
                click = e.getPoint();
                loc = square.getLocation();

            }
        }
    }

    @Override
    public void mouseDragged(MouseEvent e){
        SquareGUI square = (SquareGUI)e.getComponent();
        if(!square.emptySquare()){
            square.NeutralSquare();
            movePiece(square, e);
        }
    }

    @Override
    public void mouseReleased(MouseEvent e){
        SquareGUI square = (SquareGUI)e.getComponent();
        square.NeutralSquare();
        if(!square.emptySquare() && BoardData.get(getcell(grid.getMousePosition())).emptySquare()){
            grid.setComponentZOrder(square, zorder);
            placePiece(square, null);
        } else if(!square.emptySquare()){
            grid.setComponentZOrder(square, zorder);
            square.setLocation(loc);
        }
    }
}