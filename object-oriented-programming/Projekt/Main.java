import javax.swing.*;

public class Main {
    public static void main(String args[]){

        System.out.println("Hello"); 
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new Game();
            }
        });
    }
}
