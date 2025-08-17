abstract class Expression{
    Expression var1;
    Expression var2;
    String m;
    int value;
    abstract public int evaluate();
    abstract public void giveval(String variable, int val);
    abstract public Expression derivate();
    abstract public String toString();
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
    public Expression derivate(){
        return new Sub(var1.derivate(),var2.derivate());
    };
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
    public Expression derivate(){
        return new Add(var1.derivate(),var2.derivate());
    };
}

class Variable extends Expression{
    String variable;
    int ex,mult;

    Variable(String var,int e){
        ex = e;
        value = 0;
        variable = var;
        mult = 1;
    }
    Variable(String var,int e,int m,int v){
        ex = e;
        value = v;
        variable = var;
        mult = m;
    }
    public int evaluate(){
        return mult*(int)Math.pow(value,ex);
    }

    public String toString(){
        if (ex == 0){
            return Integer.toString(mult);
        }else if(ex == 1){
            if (mult == 1){
                return variable;
            }
            return Integer.toString(mult) + variable;
        }else{
            if (mult == 1){
                return variable + "^" + Integer.toString(ex);
            }
            return Integer.toString(mult) + variable + "^" + Integer.toString(ex);
        }
    }
    public void giveval(String variable, int val){
        if(variable == this.variable){
            value = val;
        }
    }   

    public Expression derivate(){
        if(ex == 0){
            return new Const(0);
        }
        if(ex == 1){
            return new Const(mult);
        }else{
            return new Variable(variable,(ex-1),ex*mult,value);
        }
    };
}

class Const extends Expression{
    Const(int v){
        value = v;
    }
    public int evaluate(){
        return value;
    }
    public String toString(){
        return Integer.toString(value);
    }
    public Expression derivate(){
        return new Const(0);
    };
    public void giveval(String variable, int val){};
}