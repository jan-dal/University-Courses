// JPanel with custom image

import java.awt.*;
import javax.swing.*;
import java.io.*;
import javax.imageio.ImageIO;

public class ImageGUI extends JPanel{
    Image img;

    public ImageGUI(String path){

        try {
            img = ImageIO.read(new File(path));

        } catch (IOException ex){
            System.out.println("Couldn't load image");
        }
    }
    
    
    
    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);
        int w = this.getWidth();
        int h = this.getHeight();

        // Draw the image to fit the component dimensions

        g.drawImage(img, 0, 0, w, h, this);

        
        /*
        if (w > 800 && h > 800){
            g.drawImage(chessboard, 0, 0, 800, 800, this);
        } else if (w > 800 && h <= 800){
            g.drawImage(chessboard, 0, 0, h, h, this);
        } else if (w <= w && h > 800){
            g.drawImage(chessboard, 0, 0, w, w, this);
        } else {
            g.drawImage(chessboard, 0, 0, w, h, this);
        }
        */
    }   


}
