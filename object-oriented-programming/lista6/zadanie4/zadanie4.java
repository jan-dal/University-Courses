class SortScal extends Thread{

    Element<?>[] lista;

    SortScal(Element<?>[] lista){
        this.lista = lista;
    }

    public void sortuj(){
        SortScal left,right;
        Element<?>[] result;
        Element<?> a;
        int len,s;
        len = lista.length;

        // Zamień miejscami jeżeli 0 element < 1 element
        if (len == 2 && (lista[0].compareTo(lista[1]) == -1)){
            a = this.lista[0];
            this.lista[0] = this.lista[1];
            this.lista[1] = a;
        }

        // Znajdź środek
        if(len > 2){
            if(len % 2 == 0){
                s = len / 2;
                left = new SortScal(java.util.Arrays.copyOfRange(lista,0,s));
                right = new SortScal(java.util.Arrays.copyOfRange(lista,s,len));
            }else{
                s = (len-1)/2;
                left = new SortScal(java.util.Arrays.copyOfRange(lista,0,s+1));
                right = new SortScal(java.util.Arrays.copyOfRange(lista,s+1,len));
            }
            
            left.start();
            right.start();
            try{
                left.join();
                right.join();
            }
            catch (Exception e){
                System.out.println(e);
            }

            if (len % 2 == 0){
                this.scal(right.lista,left.lista,s,len);
            }else this.scal(right.lista,left.lista,s+1,len);
        }
    }

    public void run(){
        this.sortuj();
    }

    private void scal(Element<?>[] right,Element<?>[] left,int s,int len){
        int i,j,p,b;
        i = 0;
        j = 0;
        p = 0;
        while(j < s && i < len-s){
            if(right[i].compareTo(left[j]) == -1){
                lista[p] = right[i];
                i++;
            }else{
                lista[p] = left[j];
                j++;
            }
            p++;
        }
        if(j >= s){
            while(i < len-s){
                lista[p] = right[i];
                p++;
                i++;
            }
        }else if(i >= len-s){
            while(j < s){
                lista[p] = left[j];
                p++;
                j++;
            }
        }
    }
}

class Element<T> implements Comparable<T> {
    T element;

    Element(T element){
        this.element = element;
    }

    public int compareTo(Object o){
        if(this.equals(o)){
            return 0;
        }
        return -1;
    }

}