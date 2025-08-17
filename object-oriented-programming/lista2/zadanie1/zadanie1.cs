using System;

namespace Stream {
public class IntStream {
    
    protected int liczba;

    public IntStream(){
        liczba = 0;
    }

    virtual public int next(){
        int a = liczba;
        liczba++;
        return a;

    }
    virtual public bool eos(){
        if (liczba >= int.MaxValue) {return true;}
        return false;
    }
    virtual public void reset(){
        liczba = 0;
    }

}

public class PrimeStream : IntStream {
	
	public PrimeStream(){
		liczba = 2;
	}
	
	private bool isprime(){
		for (int i=2; i < liczba; i++){
			if (liczba % i == 0){
				return false;
			}
		}
		return true;
	}
	
	private void nextprime(){
		if (this.eos() != true){
			while (true) {
				liczba++;
				if (this.isprime()){
					break;
				}
			}
		}
	}
	
	override public int next(){
		int a = liczba;
		this.nextprime();
		return a;
	
	}
	override public void reset(){
		liczba = 2;
	}

}

public class RandomStream : IntStream {
	
	new Random liczba = new Random();
	
	override public int next(){
		return this.liczba.Next();
	}
	override public bool eos(){
		return false;
	}

	override public void reset(){
	}

}

public class RandomWordStream {

	private RandomStream losuj = new RandomStream();
	private PrimeStream pierwsza = new PrimeStream();

	public string next(){
		string m = "";
		int end = pierwsza.next();
		for (int i=0;i < end;i++){
			int index = 33+(losuj.next() % 94); 
			m += (char)index;
		}
		return m;
	}
	public bool eos(){
		return pierwsza.eos();
	}
	public void reset(){
		pierwsza.reset();
	}
}
}

class Program {
    public static void Main(){
	}
}
