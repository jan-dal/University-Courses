using System;

namespace dict{

public class Element<K,V>{
    public K key;
    public V value;
    public Element<K,V> next;
    public Element(K k,V v){
        key = k;
        value = v;
    }

}

public class MyDictionary<K,V> where K : class{ 
    int size;
    Element<K,V> back,end;

    public MyDictionary(){
        size = 0;
    }

    public int dodaj(K key,V value){
        if (size == 0){
            back = new Element<K,V>(key,value);
            end = back;
            size++;
            return 0;
        }
        Element<K,V> a = back;
        while(a != null){
            if(a.key == key){
                Console.WriteLine("Klucz już istnieje");
                return 0;
            }
            a = a.next;
        }
        size++;
        end.next = new Element<K,V>(key,value);
        end = end.next;
        return 0;
    }
    public int usun(K key){

        /* Sprawdź czy pusta */

        if (size == 0){
            return 0;
        }

        /* Sprawdź czy wielkość to 1 */

        if (size == 1){
            if (back.key == key){
                back = null;
                end = null;
                size = 0;
                return 0;
            }
            Console.WriteLine("Nie znaleziono elementu");
            return 0;
        }
        Element<K,V> a = back;
        Element<K,V> b = a;

        /* Sprawdź czy element jest na początku */

        if (a.key == key){
            back = a.next;
            size--;
            return 0;
        }
        while (a.key != key){
            if(a.next == null && a.key != key){
                Console.WriteLine("Nie znaleziono elementu");
                return 0;
            }
            b = a;
            a = a.next;
        }
        size--;

        /* Sprawdź czy element jest na końcu */

        if (a.next == null){
            end = b; 
            b.next = null;
            return 0;
            }
        else {
            b.next = b.next.next;
            return 0;
        }
    }
    public V wyszukaj(K key){
        Element<K,V> a = back;
        while(a != null){
            if(a.key == key){
                return a.value;
            }
            a = a.next;
        }       
        Console.WriteLine("Nie znaleziono elementu");
        return default(V);
    }

    public bool is_empty(){
        if (size == 0){
            return true;
        }
        return false;
    }
}
}

class Program{

    public static void Main(){
    }

}