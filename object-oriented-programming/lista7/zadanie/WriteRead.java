import java.io.*;
public class WriteRead{
//Artykuł: https://www.geeksforgeeks.org/serialization-in-java/

    public void Serialize(Figure o, String filename){
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

    public Figure Deserialize(String filename){
        // Deserialization
        try
        {   
            // Reading the object from a file
            FileInputStream file = new FileInputStream(filename);
            ObjectInputStream in = new ObjectInputStream(file);
              
            // Method for deserialization of object

            Figure b = (Figure)in.readObject();
              
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