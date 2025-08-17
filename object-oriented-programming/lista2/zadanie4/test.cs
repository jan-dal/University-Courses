using System;
using List;

class Program {
    public static void Main(){
    	ListaLeniwa d = new ListaLeniwa();
    	Console.WriteLine("ListaLeniwa:");
        Console.WriteLine("Rozmiar: {0}",d.size());
        Console.WriteLine("Utworzenie 10 elementów");
        d.element(10);
        d.element(10);
        Console.WriteLine("Rozmiar: {0}",d.size());
        for (int i=0; i<d.size();i++){
            Console.WriteLine("{0}",d.element(i));
        }
        Console.WriteLine("Wywołanie .element(11)");
        d.element(11);
        Console.WriteLine("Rozmiar: {0}",d.size());
        for (int i=0; i<d.size();i++){
            Console.WriteLine("{0}",d.element(i));
        }
		Console.WriteLine("\nPierwsze:");
        Pierwsze l = new Pierwsze();
        Console.WriteLine("Rozmiar: {0}",l.size());
        Console.WriteLine("Utworzenie 10 elementów");
        l.element(1);
        l.element(1);
        Console.WriteLine("Rozmiar: {0}",l.size());
        for (int i=0; i<l.size();i++){
            Console.WriteLine("{0}",l.element(i));
        }
        Console.WriteLine("Wywołanie .element(11)");
        Console.WriteLine("Rozmiar: {0}",l.size());
        for (int i=0; i<l.size();i++){
            Console.WriteLine("{0}",l.element(i));
        }


    }

}
