class Zaszyfrowane
    def initialize(tekst)
        @tekst = tekst
    end
    def to_s()
        puts @tekst
    end

    def odszyfruj(key)
        @o = ''
        @tekst.each_char {|x| @o.concat(key.key(x))}
        Jawna.new(@o)
    end
end

class Jawna
    def initialize(tekst)
        @tekst = tekst
    end

    def to_s()
        puts @tekst
    end

    def zaszyfruj(key)
        @o = ''
        @tekst.each_char {|x| @o.concat(key[x])}
        Zaszyfrowane.new(@o)
    end
end

klucz = {'a' => 'x','b' => 'y', 'c' => 'z'}

f = Jawna.new("abc")
g = f.zaszyfruj(klucz)
g.to_s
k = g.odszyfruj(klucz)
k.to_s