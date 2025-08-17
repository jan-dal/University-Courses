public class Main {
    
    public static void main(String[] args){
        Expression l = new Add(new Variable("x",2), new Variable("x",2));
        l = new Add(new Variable("y",1),l);
        l.giveval("x",5);
        l.giveval("y",1);
        System.out.println(l.toString());
        System.out.println(l.evaluate());
    }
}
