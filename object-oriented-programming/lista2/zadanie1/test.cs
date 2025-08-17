using System;
using Stream;

class Program {
	
	public static void Main(){
		Console.WriteLine("Pierwsze 15 wyników RandomWordStream:");
		RandomWordStream a = new RandomWordStream();
		IntStream b = new IntStream();
		PrimeStream c = new PrimeStream();
		for (int i=0;i < 15;i++) Console.WriteLine("{0}",a.next());
		Console.WriteLine("Reset");
		a.reset();
		Console.WriteLine(a.next());
		Console.WriteLine(a.next());
		Console.WriteLine("Pierwsze 15 wyników IntStream");
		for (int i=0; i<15;i++) Console.WriteLine("{0}",b.next());
		Console.WriteLine("Reset");
		b.reset();
		Console.WriteLine(b.next());
		Console.WriteLine(b.next());
		Console.WriteLine("Pierwsze 15 wyników PrimeStream");
		for (int i=0; i<15;i++) Console.WriteLine("{0}",c.next());
		Console.WriteLine("Reset");
		c.reset();
		Console.WriteLine(c.next());
		Console.WriteLine(c.next());
	}
}
