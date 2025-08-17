public class MovPair {
    private int position;
    private SquareGUI piece;

    public MovPair(int i, SquareGUI p){
        position = i;
        piece = p;
    }

    public SquareGUI getSquare(){
        return piece;
    }

    public int getPos(){
        return position;
    }

}
