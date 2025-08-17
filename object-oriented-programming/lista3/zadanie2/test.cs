using System;
using dict;


class test{

    public static void Main(){
        Console.WriteLine("Utworzenie nowego słownika<string,int>");
        MyDictionary<string,int> dict = new MyDictionary<string, int>();
        Console.WriteLine("Czy słownik jest pusty? {0}", dict.is_empty());
        Console.WriteLine("Dodaj dwa - 2");
        dict.dodaj("dwa",2);
        Console.WriteLine("Czy słownik jest pusty? {0}", dict.is_empty());
        Console.WriteLine("Dodaj jeden - 1, trzy - 3\n");
        dict.dodaj("jeden",1);
        dict.dodaj("trzy",3);
        Console.WriteLine("Wyszukaj:\ndwa");
        Console.WriteLine(dict.wyszukaj("dwa"));
        Console.WriteLine("trzy");
        Console.WriteLine(dict.wyszukaj("trzy"));
        Console.WriteLine("Dodaj dwa - 4");
        dict.dodaj("dwa",4);
        Console.WriteLine("Usuń dwa");
        dict.usun("dwa");
        Console.WriteLine("Wyszukaj dwa");
        dict.wyszukaj("dwa");
        Console.WriteLine("Usuń dwa (ponownie)");
        dict.usun("dwa");
        Console.WriteLine("Usuń jeden oraz trzy");
        dict.usun("jeden");
        dict.usun("trzy");
        Console.WriteLine("Wyszukaj jeden");
        dict.wyszukaj("jeden");
        Console.WriteLine("Wyszukaj trzy");
        dict.wyszukaj("trzy");
        Console.WriteLine("Czy pusty? {0}",dict.is_empty());
        Console.WriteLine("Dodaj nowy element pięć - 5");
        dict.dodaj("pięć",5);
        Console.WriteLine("Wyszukaj pięć");
        Console.WriteLine(dict.wyszukaj("pięć"));
        Console.WriteLine("Czy pusty? {0}",dict.is_empty());
    }

}