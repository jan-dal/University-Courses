public class Main {
    
    public static void main(String[] args){
        Expression l = new Add(new Variable("x",2), new Variable("x",2));
        l = new Add(new Variable("y",1),l);
        l.giveval("x",5);
        l.giveval("y",4);
        System.out.println(l.toString());
        System.out.println(l.evaluate());
        
        Expression m = new Add(new Variable("x",4),new Variable("x",3));
        m = new Add(m,new Variable("x",2));
        m = new Add(m,new Variable("x",1));
        m = new Add(m,new Const(1));
        m.giveval("x",2);
        Expression md = m.derivate();
        Expression mdd = md.derivate();
        Expression mddd = mdd.derivate();
        Expression mdddd = mddd.derivate();
        Expression mddddd = mdddd.derivate();
        System.out.println(m.toString());
        System.out.println(m.evaluate());
        System.out.println(md.toString());
        System.out.println(md.evaluate());
        System.out.println(mdd.toString());
        System.out.println(mdd.evaluate());
        System.out.println(mddd.toString());
        System.out.println(mddd.evaluate());
        System.out.println(mdddd.toString());
        System.out.println(mdddd.evaluate());
        System.out.println(mddddd.toString());
        System.out.println(mddddd.evaluate());

        Expression o = new Add(new Const(1), new Const(0));
        o = new Sub(o,new Const(2));
        System.out.println(o.toString());
        System.out.println(o.evaluate());
        o = new Add(new Variable("x",10),o);
        o.giveval("x",10);
        System.out.println(o.toString());
        System.out.println(o.evaluate());
        Expression od = o.derivate();
        System.out.println(od.toString());
        System.out.println(od.evaluate());
    }
}
