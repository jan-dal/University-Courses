class Main {
    public static void main(String args[]){
        Lista<Integer> l = new Lista<Integer>();
        l.push_front(3);
        l.push_front(5);
        l.push_front(6);
        System.out.println(l.isEmpty());

        WriteRead<Integer> s = new WriteRead<Integer>();
        String filename = "test.tmp";
        s.Serialize(l,filename);
        Lista<Integer> p;
        p = s.Deserialize(filename);

        p.pop_back().print();
        p.pop_front().print();
        p.pop_back().print();
        System.out.println(p.isEmpty());
    }
}
