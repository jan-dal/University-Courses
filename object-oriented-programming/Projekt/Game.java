import java.awt.event.*;

// Editor mode <-> Game mode 

public class Game implements ActionListener, KeyListener{
    private boolean ingame;
    private ChessGUI game;
    private GameMovementControl k;
    
    public void keyPressed(KeyEvent k){
        int key = k.getKeyCode();
    }

    public void keyTyped(KeyEvent k){

    }
    

    public void keyReleased(KeyEvent k){

    }

    public void actionPerformed(ActionEvent e){
        String command = e.getActionCommand();
        if (command.equals("Save")){
            System.out.println("Saved");
        }
        if (command.equals("Load")){
            new LoadGameWindow();
        }
        if (command.equals("Start game")){
            if(!ingame){
                StartChessGame();
                ingame = true;
            }
        }
        if (command.equals("Stop game")){
            if(ingame){
                StopChessGame();
                ingame = false;
            }
        }
        if (command.equals("Reload board")){
            if(!ingame){
                BoardData.loadBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            }
        }
        if (command.equals("Flip board")){
            if(k != null){
                BoardData.FlipBoard();
                k.CalculateAllMoves();
            } else {
                BoardData.FlipBoard();
            }
        }


    }
    public Game(){
        BoardData.setBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        //Player player1 = new Player("Player1","w");
        //Player player2 = new Player("Player2","b");
        game = new ChessGUI();
        game.setupGUI(this);
        game.getChessboard().setFocusable(true);
        game.getChessboard().addKeyListener(this);
        game.getInfobox().getTimer1().setListener(this);
        game.getInfobox().getTimer2().setListener(this);
        EditorMovementControl k = new EditorMovementControl();
        k.setGrid(game.getChessboard().getGrid());
        SetMovementControl(k);
        ingame = false;
    }
    private void StartChessGame(){
        GameMovementControl k = new GameMovementControl();
        this.k = k;
        k.setActionListener(this);
        k.setToMove("w");
        k.setGrid(game.getChessboard().getGrid());
        k.CalculateAllMoves();
        SetStartingPos();
        SetMovementControl(k);
        game.getInfobox().getTimer1().setTimer();
        game.getInfobox().getTimer2().setTimer();
        k.setTimers(game.getInfobox().getTimer1(), game.getInfobox().getTimer2());
        if(BoardData.isFlipped()){
            game.getInfobox().getTimer1().StartTimer();
        } else {
            game.getInfobox().getTimer2().StartTimer();
        }
    }
    private void StopChessGame(){
        EditorMovementControl k = new EditorMovementControl();
        k.setGrid(game.getChessboard().getGrid());
        game.getInfobox().getTimer1().ResetTimer();
        game.getInfobox().getTimer2().ResetTimer();
        SetMovementControl(k);
    }

    private void SetStartingPos(){
        for(Piece e : BoardData.getWhite()){
            e.addHistory(e.Position);
        }
        for(Piece e : BoardData.getBlack()){
            e.addHistory(e.Position);
        }
    }

    private void SetMovementControl(MovementControl m){
        for(SquareGUI e : BoardData.getChessboard()){

            for(MouseListener i : e.getMouseListeners()){
                e.removeMouseListener(i);
            }
            for(MouseMotionListener i : e.getMouseMotionListeners()){
                e.removeMouseMotionListener(i);
            }
            e.addMouseListener(m);
            e.addMouseMotionListener(m);
        }
    }
}