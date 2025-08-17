import java.util.*;
import javax.swing.*;

public abstract class Piece implements Comparable<Piece>{
    String Side;
    ArrayList<MovPair> movelist;
    ArrayList<Integer> posHistory;
    int Position;
    boolean Moved, active;

    // Calculates moves for a piece based on movement
    public abstract ArrayList<MovPair> CalculateMoves();
    public abstract String getType();
    public abstract int getValue();
    public abstract ImageIcon getIcon();


    // Create partial list of moves
    protected ArrayList<MovPair> CreateList(int k, PositionGenerator gen){

        // k = 0 - Capture enemy move on empty squares
        // k = 1 - Move only on empty squares
        // k = 2 - Move only when capture
        // k = 3 - Move only when capturing and the captured piece just moved

        ArrayList<MovPair> list = new ArrayList<MovPair>();
        int curr = gen.next();
    
        while(!gen.END()){
            SquareGUI p = BoardData.get(curr);
            // Is empty or moved
            if((p.emptySquare() || !p.getVisible()) && k != 2 && k != 3 && !p.hasSimPiece()){
                list.add(new MovPair(curr, p));
            }
            // Is opponent
            else if(p.hasSimPiece() || (!p.emptySquare() && p.getPiece().getSide().equals(getOp(Side)))){
                if(k == 0 || k == 2){
                    list.add(new MovPair(curr, p));
                    gen.endCycle();
                }
                else if(k == 1) {
                    gen.endCycle();
                }
                else if(k == 3 && BoardData.getMoved().equals(p.getPiece()) && p.getPiece() instanceof Pawn && p.getPiece().getHistory().size() == 2){
                    list.add(new MovPair(curr, p));
                    gen.endCycle();
                }
            }
            else {
                gen.endCycle();
            }

            curr = gen.next();
        }

        return list;
    }

    public void updateMovelist(){
        movelist = CalculateMoves();
        FilterOutIllegal();
    }

    protected  String getOp(String p){
        if(p.equals("w")){
            return "b";
        } else {
            return "w";
        }
    }
    public Piece(String side, int position, boolean moved){
        movelist = new ArrayList<MovPair>();
        Side = side;
        Position = position;
        active = true;
        Moved = moved;
        posHistory = new ArrayList<Integer>();
    }  

// Getters
    public String getSide(){
        return Side;
    }
    
    public ArrayList<Integer> getHistory(){
        return posHistory;
    }

    public int getPosition(){
        return Position;
    }

    public boolean getActive(){
        return active;
    }

    public boolean Moved(){
        return Moved;
    }

    public ArrayList<MovPair> getMovelist(){
        return movelist;
    }

// Setters
    public void setSide(String side){
        Side = side;
    }

    public void addHistory(int pos){
        posHistory.add(pos);
    }

    public void resetHistory(){
        posHistory = new ArrayList<Integer>();
    }

    public void setMoved(boolean x){
        Moved = x;
    }

    public void setPosition(int pos){
        Position = pos;
    }

    public void setActive(boolean x){
        active = x;
    }

    public void setMovelist(ArrayList<MovPair> l){
        movelist = l;
    }

    @Override
    public int compareTo(Piece o) {
        if(this.getValue() > o.getValue()){
            return -1;
        } else if(this.getValue() == o.getValue()){
            return 0;
        } else {
            return 1;
        }
    }

    // Is this piece attacking a given position
    public boolean isAttacking(int pos){

        for(MovPair e : CalculateMoves()){
            if(e.getPos() == pos){
                return true;
            }
        }
        return false;
    }


    // Filters out illegal moves, does a simulation and checks if king position is being attackeg by any enemy piece
    // after a move from the movelist
    public void FilterOutIllegal(){
        int pos;
        ArrayList<MovPair> illegal = new ArrayList<MovPair>();
        // Destiny square
        SquareGUI DSquare;
        // Source square
        SquareGUI SrSquare = BoardData.get(Position);

        for(MovPair e : movelist){
            pos = Position;
            DSquare = BoardData.get(e.getPos());
            setPosition(e.getPos());
            SrSquare.setVisible(false);
            DSquare.SetSimPiece(this);
            if(!DSquare.emptySquare()){
                DSquare.getPiece().setActive(false);
            }
        
            if(BoardData.get(BoardData.getKing(Side).getPosition()).isAttacked(Side, BoardData.getKing(Side).getPosition())){
                illegal.add(e);
            }

            if(!DSquare.emptySquare()){
                DSquare.getPiece().setActive(true);
            }
            DSquare.RemoveSimPiece();
            setPosition(pos);
            SrSquare.setVisible(true);
        }

        movelist.removeAll(illegal);
    }
}

class Pawn extends Piece{
    static String Type = "P";
    static int[] atkmovementW = {1, 1, -7, -9};
    static int[] atkmovementB = {1, 1, 7, 9};
    static int[] passvmovementW = {1, 0, -8};
    static int[] passvmovementWs = {2, 0, -8};
    static int[] passvmovementBs = {2, 0, 8};
    static int[] passvmovementB = {1, 0, 8};
    static int[] enpassant = {1, 1, 1, -1};
    static ImageIcon wicon = new ImageIcon("img/Chess_" + Type + "w.png");
    static ImageIcon bicon = new ImageIcon("img/Chess_" + Type + "b.png");
    static int Value = 1;

    public Pawn(String side, int position, boolean moved){
        super(side, position, moved);
    }
    public String getType(){
        return Type;
    };
    public int getValue(){
        return Value;
    };
    public ImageIcon getIcon(){
        if(Side.equals("w")){
            return wicon;
        } else {
            return bicon;
        }
    }

    public void Promotion(){
        if((!BoardData.isFlipped() && Side.equals("w")) || (BoardData.isFlipped() && Side.equals("b"))){
            if(Position < 8){
                new ChoosePieceWindow(Position);
            }
        } else {
            if(Position > 57){
                new ChoosePieceWindow(Position);
            }
        }
    }


    // Special attacking move for pawns on their 5th rank
    private ArrayList<MovPair> enPessant(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        if((!BoardData.isFlipped() && Side.equals("w")) || (BoardData.isFlipped() && Side.equals("b"))){
            if(23 < Position && Position < 32){
                for(MovPair e : CreateList(3, new PositionGenerator(Position, enpassant))){
                    list.add(new MovPair(e.getPos() - 8, e.getSquare()));
                }
            }
        } else {
            if(31 < Position && Position < 40){
                for(MovPair e : CreateList(3, new PositionGenerator(Position, enpassant))){
                    list.add(new MovPair(e.getPos() + 8, e.getSquare()));
                }
            }
        }
        return list;
    }


    @Override
    public ArrayList<MovPair> CalculateMoves(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        if((!BoardData.isFlipped() && Side.equals("w")) || (BoardData.isFlipped() && Side.equals("b"))){
            if(Position < 56 && Position > 47){
                list.addAll(CreateList(1 ,new PositionGenerator(Position, passvmovementWs)));
            } else {
                list.addAll(enPessant());
                list.addAll(CreateList(1 ,new PositionGenerator(Position, passvmovementW)));
            }
            list.addAll(CreateList(2 ,new PositionGenerator(Position, atkmovementW)));
        } else {
            if(Position < 16 && Position > 7){
                list.addAll(CreateList(1 ,new PositionGenerator(Position, passvmovementBs)));
            } else {
                list.addAll(enPessant());
                list.addAll(CreateList(1 ,new PositionGenerator(Position, passvmovementB)));
            }
            list.addAll(CreateList(2 ,new PositionGenerator(Position, atkmovementB)));    
        }

        return list;
    }
}

class Bishop extends Piece{
    static String Type = "B";
    static int[] movement = {8, 1, 9, -9, 7, -7};
    static ImageIcon wicon = new ImageIcon("img/Chess_" + Type + "w.png");
    static ImageIcon bicon = new ImageIcon("img/Chess_" + Type + "b.png");
    static int Value = 3;
    
    public String getType(){
        return Type;
    };
    public int getValue(){
        return Value;
    };
    public ImageIcon getIcon(){
        if(Side.equals("w")){
            return wicon;
        } else {
            return bicon;
        }
    }

    public Bishop(String side, int position, boolean moved){
        super(side, position, moved);
    }

    @Override
    public ArrayList<MovPair> CalculateMoves(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        list.addAll(CreateList(0 ,new PositionGenerator(Position, movement)));
        return list;
    }


}

class Rook extends Piece{
    static String Type = "R";
    static int[] movement = {8, 1, 1, -1, 8, -8};
    static ImageIcon wicon = new ImageIcon("img/Chess_" + Type + "w.png");
    static ImageIcon bicon = new ImageIcon("img/Chess_" + Type + "b.png");
    static int Value = 5;
    
    public String getType(){
        return Type;
    };
    public int getValue(){
        return Value;
    };
    public ImageIcon getIcon(){
        if(Side.equals("w")){
            return wicon;
        } else {
            return bicon;
        }
    }

    public Rook(String side, int position, boolean moved){
        super(side, position, moved);
    }
    @Override
    public ArrayList<MovPair> CalculateMoves(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        list.addAll(CreateList(0 ,new PositionGenerator(Position, movement)));
        return list;
    }
}

class Knight extends Piece{
    static String Type = "N";
    static int[] movement = {1, 2, -10, -17, -15, -6, 10, 17, 15, 6};
    static ImageIcon wicon = new ImageIcon("img/Chess_" + Type + "w.png");
    static ImageIcon bicon = new ImageIcon("img/Chess_" + Type + "b.png");
    static int Value = 3;

    public String getType(){
        return Type;
    };
    public int getValue(){
        return Value;
    };
    public ImageIcon getIcon(){
        if(Side.equals("w")){
            return wicon;
        } else {
            return bicon;
        }
    }

    public Knight(String side, int position, boolean moved){
        super(side, position, moved);
    }

    @Override
    public ArrayList<MovPair> CalculateMoves(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        list.addAll(CreateList(0 ,new PositionGenerator(Position, movement)));
        return list;
    }
}

class Queen extends Piece{
    static String Type = "Q";
    static int[] movement = {8, 1, 9, -9, 7, -7, 8, -8, 1, -1};
    static ImageIcon wicon = new ImageIcon("img/Chess_" + Type + "w.png");
    static ImageIcon bicon = new ImageIcon("img/Chess_" + Type + "b.png");
    static int Value = 9;
    
    public String getType(){
        return Type;
    };
    public int getValue(){
        return Value;
    };
    public ImageIcon getIcon(){
        if(Side.equals("w")){
            return wicon;
        } else {
            return bicon;
        }
    }
    public Queen(String side, int position, boolean moved){
        super(side, position, moved);
    }
    @Override
    public ArrayList<MovPair> CalculateMoves(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        list.addAll(CreateList(0 ,new PositionGenerator(Position, movement)));
        return list;
    }

}

class King extends Piece{

    static String Type = "K";
    static int[] movement = {1, 1, 9, -9, 7, -7, 8, -8, 1, -1};
    static ImageIcon wicon = new ImageIcon("img/Chess_" + Type + "w.png");
    static ImageIcon bicon = new ImageIcon("img/Chess_" + Type + "b.png");
    static int Value = 100;
    private boolean inCheck;
    
    public String getType(){
        return Type;
    };
    public int getValue(){
        return Value;
    };
    public ImageIcon getIcon(){
        if(Side.equals("w")){
            return wicon;
        } else {
            return bicon;
        }
    }
    public King(String side, int position, boolean moved){
        super(side, position, moved);
    }

    @Override
    public ArrayList<MovPair> CalculateMoves(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        list.addAll(CreateList(0 ,new PositionGenerator(Position, movement)));
        return list;
    } 

    @Override
    public void updateMovelist() {
        movelist = CalculateMoves();
        movelist.addAll(CastleKingside());
        movelist.addAll(CastleQueenside());
        FilterOutIllegal();
    }

    private ArrayList<MovPair> CastleKingside(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        if((!BoardData.isFlipped() && Side.equals("w")) || (BoardData.isFlipped() && Side.equals("b"))){
            if(!Moved && !inCheck && BoardData.get(61).emptySquare() && BoardData.get(62).emptySquare() && !BoardData.get(63).emptySquare()){
                if(!BoardData.get(63).getPiece().Moved() && !BoardData.get(61).isAttacked(Side, 61) && !BoardData.get(62).isAttacked(Side, 62)){
                    list.add(new MovPair(62, BoardData.get(62)));
                }
            }
        } else {
            if(!Moved && !inCheck && BoardData.get(5).emptySquare() && BoardData.get(6).emptySquare() && !BoardData.get(7).emptySquare()){
                if(!BoardData.get(7).getPiece().Moved() && !BoardData.get(5).isAttacked(Side, 5) && !BoardData.get(6).isAttacked(Side, 6)){
                    list.add(new MovPair(6, BoardData.get(6)));
                }
            }
        }

        return list;
    }

    private ArrayList<MovPair> CastleQueenside(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        if((!BoardData.isFlipped() && Side.equals("w")) || (BoardData.isFlipped() && Side.equals("b"))){
            if(!Moved && !inCheck && BoardData.get(59).emptySquare() && BoardData.get(58).emptySquare() && BoardData.get(57).emptySquare() && !BoardData.get(56).emptySquare()){
                if(!BoardData.get(56).getPiece().Moved() && !BoardData.get(57).isAttacked(Side, 57) && !BoardData.get(58).isAttacked(Side, 58) && !BoardData.get(59).isAttacked(Side, 59)){
                    list.add(new MovPair(58, BoardData.get(58)));
                }
            }
        } else {
            if(!Moved && !inCheck && BoardData.get(1).emptySquare() && BoardData.get(2).emptySquare() && BoardData.get(3).emptySquare() && !BoardData.get(0).emptySquare()){
                if(!BoardData.get(0).getPiece().Moved() && !BoardData.get(1).isAttacked(Side, 1) && !BoardData.get(2).isAttacked(Side, 2) && !BoardData.get(3).isAttacked(Side, 3)){
                    list.add(new MovPair(2, BoardData.get(2)));
                }
            }
        }

        return list;
    }


    public void setInCheck(boolean x){
        inCheck = x;
    }

    public boolean inCheck(){
        return inCheck;
    }
}

// A passive queen
class Trapper extends Piece{
    static String Type = "T";
    static int[] movement = {8, 1, 9, -9, 7, -7, 8, -8, 1, -1};
    static ImageIcon wicon = new ImageIcon("img/Chess_" + Type + "w.png");
    static ImageIcon bicon = new ImageIcon("img/Chess_" + Type + "b.png");
    static int Value = 3;

    public String getType(){
        return Type;
    };
    public int getValue(){
        return Value;
    };
    public ImageIcon getIcon(){
        if(Side.equals("w")){
            return wicon;
        } else {
            return bicon;
        }
    }
    public Trapper(String side, int position, boolean moved){
        super(side, position, moved);
    }

    @Override
    public ArrayList<MovPair> CalculateMoves(){
        ArrayList<MovPair> list = new ArrayList<MovPair>();
        list.addAll(CreateList(1 ,new PositionGenerator(Position, movement)));
        return list;
    }
}
