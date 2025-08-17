using System;
using System.Collections;

namespace primecollection{

class Element{

    public int n;

    public Element(){
        n = 1;
    }

    private bool primecheck(){
        
        for(int i=2; i < (int)Math.Sqrt(n) + 1; i++){
            if(n%i == 0){
                return false;
            }
        }
        return true;
    }

    public int next(){
        n++;
        while (!primecheck()){
            if(n < 0) return n;
            n++;
        }
        return n;
    }

    public void reset(){
        n = 1;
    }

}


class ListEnum : IEnumerator{

    Element current;
    public ListEnum(Element prime){
        this.current = prime;
    }

    public bool MoveNext(){
        return this.current.next() > 0;
    }

    public object Current{
        get{
            return current.n;
        }
    }

    public void Reset(){
        this.current.reset();
    }

}


public class PrimeCollection :IEnumerable{

    Element prime = new Element();

    public IEnumerator GetEnumerator(){
        return new ListEnum(prime);
    }
}
}