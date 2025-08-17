import java.io.*;
class Figure implements Serializable {
    static final long serialVersionUID = 0L;
    int x,y;
    String type, color;
    public Figure(int x, int y, String c){
        this.x = x;
        this.y = y;
        color = c;
    }
    public String toString(){
        String m;
        m = "  Position: " + "(" + Integer.toString(x) + "," + Integer.toString(y) + ")\n";
        m += "  Color: " + color;
        return m;
    }
}

class Circle extends Figure{
    int radius;
    public Circle(int x, int y, String c, int r){
        super(x,y,c);
        radius = r;
        type = "Circle";
    }
    public String toString(){
        String m;
        m = type + "\n";
        m += super.toString();
        m += "\n  Radius: " + Integer.toString(radius);
        return m;
    }
}

class Triangle extends Figure{
    int height, base;
    public Triangle(int x, int y, String c, int h, int b){
        super(x,y,c);
        height = h;
        base = b;
        type = "Triangle";
    }
    public String toString(){
        String m;
        m = type + "\n";
        m += super.toString();
        m += "\n  Base: " + Integer.toString(base) + "\n";
        m += "  Height: " + Integer.toString(height);
        return m;
    }
}