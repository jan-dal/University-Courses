// Shows pieces on a chess BoardData
import java.awt.*;
import javax.swing.*;

public class ChessBoardGUI extends JPanel{
    ImageGUI grid;

    public ChessBoardGUI(){
        // Setup chessBoardData and sidebars images
        //this.setMinimumSize(new Dimension(800, 800));
        this.setPreferredSize(new Dimension(800, 800));
        this.setBackground(new Color(49,46,43));

        this.setLayout(new BoxLayout(this,BoxLayout.Y_AXIS));

        ImageGUI bottombar = new ImageGUI("img/bottombar.png");
        bottombar.setPreferredSize(new Dimension(800,32));
        ImageGUI sidebar = new ImageGUI("img/leftbar.png");
        sidebar.setMaximumSize(new Dimension(32, 4000));
        sidebar.setPreferredSize(new Dimension(32,768));
        bottombar.setMinimumSize(new Dimension(0, 32));
        bottombar.setMaximumSize(new Dimension(4000,32));
        sidebar.setMinimumSize(new Dimension(32,0));

        JPanel contentpanel = new JPanel();
        contentpanel.setLayout(new BoxLayout(contentpanel, BoxLayout.X_AXIS));
        contentpanel.setBackground(new Color(49,46,43));
        
        this.add(contentpanel);
        this.add(bottombar);

        contentpanel.add(sidebar);

        grid = new ImageGUI("img/chessboard.png");
        grid.setLayout(new GridLayout(8,8,0,0));

        LoadPieces();

        contentpanel.add(grid);
    }
    
    public void LoadPieces(){
        for(SquareGUI e : BoardData.getChessboard()){
            grid.add(e);
        }
    }


    public ImageGUI getGrid(){
        return grid;
    }
}
