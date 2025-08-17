import javax.swing.*;
import javax.swing.text.*;
import java.awt.event.*;
import java.awt.Color;
import java.awt.Font;

public class TimerGUI extends JPanel implements ActionListener{
    private Timer timer;
    private int h, min, s;
    private ActionListener listener;
    JFormattedTextField DisplayTime;

    public void actionPerformed(ActionEvent e){
        updateTime();
    }

    // Format 00 : 00 : 00
    public TimerGUI(){
        MaskFormatter mf;

        this.setLayout(new BoxLayout(this,BoxLayout.Y_AXIS));


        timer = new Timer(1000, this);
        try {
            mf = new MaskFormatter("## : ## : ##");
            mf.setPlaceholderCharacter('#');
            DisplayTime = new JFormattedTextField(mf);
            DisplayTime.setColumns(6);
        } catch (java.text.ParseException exc){
            System.out.println("Parse exeption");
        }

        h = 0;
        min = 0;
        s = 0;
        DisplayTime.setHorizontalAlignment(JTextField.CENTER);
        DisplayTime.setBackground(new Color(49,46,43));
        DisplayTime.setForeground(new Color(255,255,255));
        DisplayTime.setFont(new Font("Monospaced", Font.PLAIN, 40));
        DisplayTime.setBorder(javax.swing.BorderFactory.createEmptyBorder());
        DisplayTime.setText("00 : 00 : 00");

        add(Box.createHorizontalGlue());
        add(DisplayTime);
        add(Box.createHorizontalGlue());
    }

    public void StartTimer(){
        DisplayTime.setEditable(false);
        if(h != 0 || min != 0 || s != 0){
            timer.start();
        }
    }

    public void ResetTimer(){
        timer.stop();
        DisplayTime.setEditable(true);
        DisplayTime.setText("00 : 00 : 00");
    }

    public boolean isRunning(){
        return timer.isRunning();
    }

    public void setTimer(){
        timer.stop();
        String m = DisplayTime.getText();
        h = Integer.parseInt(m.substring(0,2));
        min = Integer.parseInt(m.substring(5,7));
        s = Integer.parseInt(m.substring(10,12));

        DisplayTime.setText(getTimeStr(h) + " : " + getTimeStr(min) + " : " + getTimeStr(s));
    }

    public void StopTimer(){
        timer.stop();
    }

    private void updateTime(){
        s = ((((s - 1) % 60) + 60) % 60);

        if (s == 59){
            min = ((((min - 1) % 60) + 60) % 60);
        }
        if (min == 59 && s == 59){
            h = ((((h - 1) % 60) + 60) % 60);
        }

        if (min == 0 && s == 0 && h == 0){
            timer.stop();
            listener.actionPerformed(new ActionEvent(this, 0, "Stop game"));
            new GameEndWindow(1);
        } else {

            DisplayTime.setText(getTimeStr(h) + " : " + getTimeStr(min) + " : " + getTimeStr(s));
            DisplayTime.repaint();
        }
    }

    private String getTimeStr(int t){
        String out = Integer.toString(t);
        if(t < 10){
            return "0" + out;
        } else {
            return out;
        }
    }

    public void setListener(ActionListener l){
        listener = l;
    }
    
}
