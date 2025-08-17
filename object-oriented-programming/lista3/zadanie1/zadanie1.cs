using System;


namespace list{




public class Element<T> {

    public Element<T> next,bef;
    public T value;

    public Element(T val,Element<T> b,Element<T> n){
        value = val;
        bef = b;
        next = n;
    }
}


public class Lista<T> {
    int s;
    public Element<T> back,front;

    public void push_back(T elem){
        s++;
        if(back == null){
            back = new Element<T>(elem,null,null);
            front = back;
        } else {
            back.bef = new Element<T>(elem,null,back);
            back = back.bef;
            }
    }

    public void push_front(T elem){
        s++;
        if(front == null){
            front = new Element<T>(elem,null,null);
            back = front;
        } else {
            front.next = new Element<T>(elem,front,null);
            front = front.next;
            }
    }

    public Element<T> pop_front(){
        if(front == null){
            return null;
        }
        else {
            if(s == 1){
                Element<T> a = front;
                back = null;
                front = null;
                s--;
                return a;
            }
            else{
                front = front.bef;
                s--;
                return front.next;
            }
        }
    }

    public Element<T> pop_back(){
        if(back == null){
            return null;
        }
        else {
            if(s == 1){
                Element<T> a = back;
                back = null;
                front = null;
                s--;
                return a;
            }
            else{
                back = back.next;
                s--;
                return back.bef;
            }
        }  
    }

    public bool is_empty(){
        if(front == null && back == null){
            return true;
        }
        return false;
    }
}
}

class Program {
    public static void Main(){
    }
}

