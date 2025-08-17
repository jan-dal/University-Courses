abstract class Expression{
    Expression var1;
    Expression var2;
    String m;
    public int evaluate(){
        return 0;
    }
    public void giveval(String variable, int val){
    }
}

class Sub extends Expression{
    Sub(Expression v1,Expression v2){
        var1 = v1;
        var2 = v2;
        m = "";
    }
    public int evaluate(){
        return var1.evaluate() - var2.evaluate();
    }
    public String toString(){
        if (m == ""){
        m += var1.toString();
        m += "-";
        m += var2.toString();
        }
        return m;
    }
    public void giveval(String variable, int val){
        var1.giveval(variable, val);
        var2.giveval(variable, val);
    }   
}

class Add extends Expression{
    Add(Expression v1,Expression v2){
        var1 = v1;
        var2 = v2;
        m = "";
    }
    public int evaluate(){
        return var1.evaluate()+var2.evaluate();
    }

    public String toString(){
        if (m == ""){
        m += var1.toString();
        m += "+";
        m += var2.toString();
        }
        return m;
    }

    public void giveval(String variable, int val){
        var1.giveval(variable, val);
        var2.giveval(variable, val);
    }
}

class Mult extends Expression{
    Mult(Expression v1,Expression v2){
        var1 = v1;
        var2 = v2;
        m = "";
    }

    public int evaluate(){
        return var1.evaluate()*var2.evaluate();
    }

    public String toString(){
        if(m == ""){
        m += var1.toString();
        if (!(var2 instanceof Variable)){m+= "*";}
        m += var2.toString();
        }
    return m;
    }

    public void giveval(String variable, int val){
        var1.giveval(variable, val);
        var2.giveval(variable, val);
    }   
}

class Variable extends Expression{
    String variable;
    int value,ex;

    Variable(String var,int e){
        ex = e;
        value = 0;
        variable = var;
    }
    public int evaluate(){
        return (int)Math.pow(value,ex);
    }

    public String toString(){
        if (ex == 0){
            return "1";
        }else if(ex == 1){
            return variable;
        }
        else{
            return variable + "^" + Integer.toString(ex);
        }
    }
    public void giveval(String variable, int val){
        if(variable == this.variable){
            value = val;
        }
    }   
}

class Const extends Expression{
    int value;
    
    Const(int v){
        value = v;
    }
    public int evaluate(){
        return value;
    }
    public String toString(){
        return Integer.toString(value);
    }
}