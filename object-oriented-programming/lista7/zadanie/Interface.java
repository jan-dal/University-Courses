import javax.swing.*;

public class Interface{
    EditFigure edytor;
    JFrame frame;
    public Interface(Figure f){
        frame = new JFrame("Edycja figur");
        edytor = new EditFigure(f);
        frame.getContentPane().add(edytor);
        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}
