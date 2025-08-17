// Shows information about the game
import java.awt.*;
import javax.swing.*;

public class InformationGUI extends JPanel{
    JLabel player1, player2;
    TimerGUI timer1, timer2;
    
    public InformationGUI(){
        this.setPreferredSize(new Dimension(440,800));
        this.setBackground(new Color(49,46,43));
        this.setLayout(new GridLayout(4,0));

        player1 = new JLabel("Player 1");
        player2 = new JLabel("Player 2");
        player1.setFont(new Font("Monospaced", Font.PLAIN, 25));
        player2.setFont(new Font("Monospaced", Font.PLAIN, 25));
        player1.setForeground(new Color(255, 255, 255));
        player2.setForeground(new Color(255, 255, 255));
        player1.setHorizontalAlignment(JLabel.CENTER);
        player2.setHorizontalAlignment(JLabel.CENTER);
        
        timer1 = new TimerGUI();
        timer2 = new TimerGUI();




        add(player1);
        add(timer1);
        add(timer2);
        add(player2);


    }
    public TimerGUI getTimer1(){
        return timer1;
    }
    public TimerGUI getTimer2(){
        return timer2;
    }
    public void setPlayer1(String name){
        player1.setText(name);
    }
    public void setPlayer2(String name){
        player2.setText(name);
    }


}