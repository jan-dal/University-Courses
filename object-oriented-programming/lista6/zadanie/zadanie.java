import java.io.*;
class Element<T> implements Serializable{
    static final long serialVersionUID = 0L;
    public Element<T> next,bef;
    T value;
    
    public Element(T val,Element<T> b,Element<T> n){
        value = val;
        bef = b;
        next = n;
    }
    public void print(){
        System.out.println(value);
    }

}


class Lista<T> implements Serializable{
    int s;
    static final long serialVersionUID = 1L;
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

    public boolean isEmpty(){
        if(front == null && back == null){
            return true;
        }
        return false;
    }
}
class WriteRead<T>{
//Artykuł: https://www.geeksforgeeks.org/serialization-in-java/

    public void Serialize(Lista<T> o, String filename){
        try
        {   
            //Saving of object in a file
            FileOutputStream file = new FileOutputStream(filename);
            ObjectOutputStream out = new ObjectOutputStream(file);
            // Method for serialization of object
            out.writeObject(o);
              
            out.close();
            file.close();
              
            System.out.println("Object has been serialized");
  
        }
        catch(IOException ex)
        {
            System.out.println("IOException is caught");
        }
    }

    public Lista<T> Deserialize(String filename){
        // Deserialization
        try
        {   
            // Reading the object from a file
            FileInputStream file = new FileInputStream(filename);
            ObjectInputStream in = new ObjectInputStream(file);
              
            // Method for deserialization of object
            @SuppressWarnings("unchecked")
            Lista<T> b = (Lista<T>)in.readObject();
              
            in.close();
            file.close();
              
            System.out.println("Object has been deserialized ");
            return b;
        }
          
        catch(IOException ex)
        {
            System.out.println("IOException is caught");
        }
          
        catch(ClassNotFoundException ex)
        {
            System.out.println("ClassNotFoundException is caught");
        }
        return null;
    }
}