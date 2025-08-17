using System;

namespace List {
public class ListaLeniwa {
    protected int s;
    protected int[] lista = new int[0];
    protected Random n = new Random();
    
    virtual public int element(int i){
        if (i < s){
            return lista[i];
        }
        Array.Resize<int>(ref lista, i);
        for (int p=s; p<i; p++){
            lista[p]=n.Next();
        }
        s = i;
        return lista[s-1];

    }
    public int size(){
        return s;
    }


}

public class Pierwsze : ListaLeniwa{
    new int n = 2;
	private bool isprime(){
		for (int i=2; i < n; i++){
			if (n % i == 0){
				return false;
			}
		}
		return true;
	}
	
	private void nextprime(){
		while (true) {
			n++;
			if (this.isprime()){
				break;
			}
		}
	}


    public override int element(int i) {
        if (i < s){
            return lista[i];
        }
        Array.Resize<int>(ref lista, i);
        for (int p=s; p<i; p++){
            lista[p]=n;
            nextprime();
        }
        s = i;
        return lista[s-1]; 
    }
}
}
class Program {
    public static void Main(){
        }
    }
