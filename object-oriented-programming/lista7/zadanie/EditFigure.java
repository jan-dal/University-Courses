import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class EditFigure extends JComponent implements ActionListener {
    static final long serialVersionUID = 1L;
    Figure f;
    JTextField t_x,t_y,t_h,t_b,t_r,t_c;
    JLabel l_type,l_x,l_y,l_h,l_b,l_r,l_c;
    JButton button;
    public EditFigure(Figure f){
        this.f = f;
        GridBagLayout grid = new GridBagLayout();
        GridBagConstraints c = new GridBagConstraints();
        setLayout(grid);

        Font font = new Font("Verdana",Font.PLAIN, 50);

        setFont(font);
        // Labels

        l_type = new JLabel(f.type);
        l_x = new JLabel("x");
        l_y = new JLabel("y");
        l_c = new JLabel("Color");
        l_type.setFont(font);
        l_x.setFont(font);
        l_y.setFont(font);
        l_c.setFont(font);

        // Text fields

        t_x = new JTextField(Integer.toString(f.x), 10);
        t_y = new JTextField(Integer.toString(f.y), 10);
        t_c = new JTextField(f.color, 15);
        t_x.setFont(font);
        t_y.setFont(font);
        t_c.setFont(font);

        // Buttons

        button = new JButton("Save");
        button.addActionListener(this);
        button.setFont(font);

        c.weightx = 1.0;
        c.gridx = 0;
        c.gridy = 0;
        c.ipady = 30;
        c.fill = GridBagConstraints.CENTER;
        c.anchor = GridBagConstraints.PAGE_START;
        c.gridwidth = 2;
        grid.setConstraints(l_type, c);
        c.fill = GridBagConstraints.BOTH;
        c.gridwidth = 1;
        c.gridx = 0;
        c.gridy++;
        c.ipadx = 10;
        grid.setConstraints(l_x, c);
        c.gridx = 1;
        grid.setConstraints(t_x, c);
        c.gridy++;
        c.gridx = 0;
        grid.setConstraints(l_y, c);
        c.gridx = 1;
        grid.setConstraints(t_y, c);
        c.gridy++;
        c.gridx = 0;
        grid.setConstraints(l_c, c);
        c.gridx = 1;
        grid.setConstraints(t_c, c);
        c.gridy++;
        c.gridx = 0;


        
        if (f.type.equals("Circle")){
            l_r = new JLabel("Radius");
            t_r = new JTextField(Integer.toString(((Circle)f).radius), 10);
            l_r.setFont(font);
            t_r.setFont(font);
            grid.setConstraints(l_r, c);
            c.gridx = 1;
            grid.setConstraints(t_r, c);
            add(l_r);
            add(t_r);
        }
        if (f.type.equals("Triangle")){
            l_b = new JLabel("Base");
            t_b = new JTextField(Integer.toString(((Triangle)f).base), 10);
            l_h = new JLabel("Height");
            t_h = new JTextField(Integer.toString(((Triangle)f).height), 10);
            l_b.setFont(font);
            l_h.setFont(font);
            t_b.setFont(font);
            t_h.setFont(font);
            grid.setConstraints(l_b, c);
            c.gridx = 1;
            grid.setConstraints(t_b, c);
            c.gridy++;
            c.gridx = 0;
            grid.setConstraints(l_h, c);
            c.gridx = 1;
            grid.setConstraints(t_h, c);
            add(l_b);
            add(t_b);
            add(l_h);
            add(t_h);
        }

        c.gridy++;
        c.gridx = 0;
        c.anchor = GridBagConstraints.PAGE_END;
        c.gridwidth = 2;
        grid.setConstraints(button, c);

        add(l_type);
        add(l_x);
        add(l_y);
        add(l_c);
        add(t_y);
        add(t_x);
        add(t_c);
        add(button);
    }

    public void actionPerformed(ActionEvent e){
        f.x = Integer.parseInt(t_x.getText());
        f.y = Integer.parseInt(t_y.getText());
        f.color = t_c.getText();
        
        if (f.type.equals("Circle")){
            ((Circle)f).radius = Integer.parseInt(t_r.getText());
            }
        if (f.type.equals("Triangle")){
            ((Triangle)f).base = Integer.parseInt(t_b.getText());
            ((Triangle)f).height = Integer.parseInt(t_h.getText());
        }
    }
}
