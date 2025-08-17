using System;
using list;

class test{

    public static void Main(){
        Console.WriteLine("Nowa int lista");
        Lista<int> list = new Lista<int>();
        Console.WriteLine("Czy pusta? {0}",list.is_empty());
        Console.WriteLine("Push back 3 2 1; Push front 4 5 6");
        list.push_back(3);
        list.push_back(2);
        list.push_back(1);
        list.push_front(4);
        list.push_front(5);
        list.push_front(6);
        Console.WriteLine("Czy pusta? {0}",list.is_empty());

        Console.WriteLine("list.pop_back().value {0}",list.pop_back().value);
        Console.WriteLine("list.pop_front().value {0}",list.pop_front().value);
        Console.WriteLine("list.pop_back().value {0}",list.pop_back().value);
        Console.WriteLine("list.pop_front().value {0}",list.pop_front().value);
        Console.WriteLine("list.pop_back().value {0}",list.pop_back().value);
        Console.WriteLine("list.pop_back().value {0}",list.pop_back().value);

        Console.WriteLine("Czy pusta? {0}",list.is_empty());
        
        Console.WriteLine("Nowa string lista");
        Lista<string> list2 = new Lista<string>();
        Console.WriteLine("Czy pusta? {0}",list2.is_empty());
        Console.WriteLine("Push back c b a; Push front d e f");
        list2.push_back("c");
        list2.push_back("b");
        list2.push_back("a");
        list2.push_front("d");
        list2.push_front("e");
        list2.push_front("f");
        Console.WriteLine("Czy pusta? {0}",list2.is_empty());

        Console.WriteLine("list.pop_back().value {0}",list2.pop_back().value);
        Console.WriteLine("list.pop_front().value {0}",list2.pop_front().value);
        Console.WriteLine("list.pop_back().value {0}",list2.pop_back().value);
        Console.WriteLine("list.pop_front().value {0}",list2.pop_front().value);
        Console.WriteLine("list.pop_back().value {0}",list2.pop_back().value);
        Console.WriteLine("list.pop_back().value {0}",list2.pop_back().value);

        Console.WriteLine("Czy pusta? {0}",list2.is_empty());
        
    }
}