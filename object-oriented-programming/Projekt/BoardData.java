// Holds information about the status of pieces
import java.util.*;

public class BoardData {
    // 1D array representing the chessboard
    private static SquareGUI[] Chessboard;
    // 
    private static boolean flipped;
    private static ArrayList<Piece> White, Black;
    private static Piece Moved;

    // Initialize chessboard from fen string
    public static void setBoard(String fen){
        flipped = false;
        if(!checkFen(fen)){
            System.out.println("Bad fen code");
        } else {
        
        Piece tempPiece;
        Chessboard = new SquareGUI[64];
        White = new ArrayList<Piece>();
        Black = new ArrayList<Piece>();
        
    // Convert fen string to array
        int i = 0;
        for(char x : fen.toCharArray()){
            if(Character.isWhitespace(x)){
                break;
            }
            else if(Character.isLetter(x)){
                if(Character.isLowerCase(x)){
                    tempPiece = CreatePiece(Character.toString(Character.toUpperCase(x)), "b", i);
                    Chessboard[i] = new SquareGUI();
                    Chessboard[i].SetPiece(tempPiece);
                    Black.add(tempPiece);
                } else {
                    if(x == 'K'){
                    }
                    tempPiece = CreatePiece(Character.toString(x),"w", i);
                    Chessboard[i] = new SquareGUI();
                    Chessboard[i].SetPiece(tempPiece);
                    White.add(tempPiece);
                }
            }
            else if(Character.isDigit(x)){
                int p = i;
                for(int k = 0; k < Character.getNumericValue(x); k++){
                    Chessboard[p+k] = new SquareGUI();
                    i++;
                }
                i--;
            }
            if(x != '/'){
                i++;
            }
        }
        Collections.sort(White);
        Collections.sort(Black);
        }
    }

    public static void loadBoard(String fen){
        if(!checkFen(fen)){
            System.out.println("Bad fen code");
        } else {
        
        Piece tempPiece;
        White = new ArrayList<Piece>();
        Black = new ArrayList<Piece>();
        
    // Convert fen string to array

        int i = 0;
        for(char x : fen.toCharArray()){
            if(Character.isWhitespace(x)){
                break;
            }
            else if(Character.isLetter(x)){
                if(Character.isLowerCase(x)){
                    tempPiece = CreatePiece(Character.toString(Character.toUpperCase(x)), "b", i);
                    Chessboard[i].SetPiece(tempPiece);
                    Black.add(tempPiece);
                } else {
                    if(x == 'K'){
                    }
                    tempPiece = CreatePiece(Character.toString(x),"w", i);
                    Chessboard[i].SetPiece(tempPiece);
                    White.add(tempPiece);
                }
            }
            else if(Character.isDigit(x)){
                int p = i;
                for(int k = 0; k < Character.getNumericValue(x); k++){
                    Chessboard[p+k].RemovePiece();
                    i++;
                }
                i--;
            }
            if(x != '/'){
                i++;
            }
        }
        Collections.sort(White);
        Collections.sort(Black);
        }
    }

    public static Piece getKing(String side){
        if(side.equals("w")){
            return White.get(0);
        } else {
            return Black.get(0);
        }
    }

    public static SquareGUI get(int cell){
        return Chessboard[cell];
    }

    public static ArrayList<Piece> getWhite(){
        return White;
    }

    public static ArrayList<Piece> getBlack(){
        return Black;
    }

    public static SquareGUI[] getChessboard(){
        return Chessboard;
    }

    public static void setMoved(Piece p){
        Moved = p;
    }

    public static Piece getMoved(){
        return Moved;
    }

    public static Piece CreatePiece(String type, String Side, int pos){

        switch(type){
            case "P" : return new Pawn(Side, pos, false);
            case "B" : return new Bishop(Side, pos, false);
            case "N" : return new Knight(Side, pos, false);
            case "R" : return new Rook(Side, pos, false);
            case "Q" : return new Queen(Side, pos, false);
            case "K" : return new King(Side, pos, false);
            case "T" : return new Trapper(Side, pos, false);
            default : return null;
        }
    }

    public static boolean isFlipped(){
        return flipped;
    }

    public static void Remove(Piece e){
        if(e != null){
            if(e.getSide().equals("w")){
                White.remove(e);
            } else {
                Black.remove(e);
            }
        }
    }

    public static void Add(Piece e){
        if(e.getSide().equals("w")){
            White.add(e);
            Collections.sort(White);
        } else {
            Black.add(e);
            Collections.sort(Black);
        }
    }

    private static boolean checkFen(String fen){
        int len = 0;
        int c = 0;
        for(char x : fen.toCharArray()){
            if(Character.isAlphabetic(x)){
                len++;
            } else if(x == '/'){
                c++;
            } else if(Character.isDigit(x)){
                len += Character.getNumericValue(x);
            } else {
                break;
            }
        }
        if(len == 64 && c == 7){
            return true;
        }
        return false;

    }

    public static void FlipBoard(){
        for(SquareGUI e : Chessboard){
            e.RemovePiece();
        }

        int rank,x,newPos,newHis;
        ArrayList<Integer> his;

        for(Piece e : White){
            if (e.getActive()){
                rank = (int)e.getPosition()/8;
                x = e.getPosition() % 8;
                newPos = e.getPosition() - ((2*rank-7)*8 + (2*x - 7));
                his = e.getHistory();
                e.resetHistory();
                for(int j : his){
                    newHis = j - ((2*rank-7)*8 + (2*x - 7));
                    e.addHistory(newHis);
                }
                e.setPosition(newPos);
                Chessboard[newPos].SetPiece(e);
                }
        }

        for(Piece e : Black){
            if(e.getActive()){
                rank = (int)e.getPosition()/8;
                x = e.getPosition() % 8;
                newPos = e.getPosition() - ((2*rank-7)*8 + (2*x - 7));
                e.setPosition(newPos);
                his = e.getHistory();
                e.resetHistory();
                for(int j : his){
                    newHis = j - ((2*rank-7)*8 + (2*x - 7));
                    e.addHistory(newHis);
                }
                e.setPosition(newPos);
                Chessboard[newPos].SetPiece(e);
            }
        }

        flipped = !flipped;
    }

}