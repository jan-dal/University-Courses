using System;
class Array {

    public int size, index, value;
    private int left, right;
    private next = null; 
    private prev = null;

    private void expand(int num){
        while (this.next != null){
            this.next = this.next.next;
        }
        for (int i=0;i<num;i++){
            Array next = new Array(this, left, right, index++, 0);
        }
    }
    public Array(int beg, int end){
        size = end-beg+1;
        left = beg;
        right = end;
        index = beg;
        value = 0;
        Array next = new Array(this, beg, end, index+1 ,0);

    }

    public Array(Array p, int beg, int end, int i, int val){
        size = end-beg+1;
        left = beg;
        right = end;
        index = i;
        value = val;
        prev = p;
        
        if (i < right){
            next = new Array(this, beg, end, index+1 ,0);
        }
    }

    public void ch_size(int new_left, int new_right){
        left = new_left;
        right = new_right;
        size = right-left+1;
    }

    public void set(int i, int val){
        if (i < left || i > right){
            Console.WriteLine("Indeks poza rozmiarem tablicy.");
        }else{
            while(this.index != i){
                this.next = this.next.next;
            }
            this.value = val;
        
        }

    }
}
class Program {

    public static void Main(){
        Array l = new Array(0, 10);
        Console.WriteLine("{0}",l.next.next.value);
        l.set(2,10);
    }

}