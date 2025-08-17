import java.io.*;

public class Main{
    String filename,classname;
    Figure o;
    WriteRead save = new WriteRead();
    public static void main(String args[]){
        new Main(args[0],args[1]);
    }
    Main(String a, String b){
        filename = a;
        classname = b;
        File f = new File(filename);

        if (f.exists()){
            o = save.Deserialize(filename);
            Interface edit = new Interface(o);
            edit.frame.addWindowListener(new java.awt.event.WindowAdapter() {
                @Override
                public void windowClosing(java.awt.event.WindowEvent windowEvent) {
                    save.Serialize(o,filename);
                }
            });
        }else{
            if(classname.equals("Circle")){
                Circle t = new Circle(0,0,"White",1);
                save.Serialize(t,filename);
            }
            if(classname.equals("Triangle")){
                Triangle t = new Triangle(0,0,"White",1,1);
                save.Serialize(t,filename);
            }
        }
    }
}
