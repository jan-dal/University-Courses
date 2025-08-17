class Main{

    public static void main(String[] args){
        Element<?>[] lista = new Element<?>[1000];
        for (int i = 0 ; i < 1000; i++){
            lista[i] = new Element<Integer>(i);
            //System.out.println(lista[i].element);
        }
        System.out.println("-------");
        SortScal scal = new SortScal(lista); 
        scal.sortuj();
        for (int i = 0 ; i < 1000; i++){
            System.out.println(scal.lista[i].element);
        }
        System.out.println();

        lista = new Element<?>[10];
        for (int i = 0 ; i < 10; i++){
            lista[i] = new Element<Character>((char)(i+65));
            System.out.println(lista[i].element);
        }
        System.out.println("-------");
        scal = new SortScal(lista); 
        scal.sortuj();
        for (int i = 0 ; i < 10; i++){
            System.out.println(scal.lista[i].element);
        }
    }
}