class Element
    attr_reader :val
    def initialize(v)
        @val = v
        @next = nil
        @prev = nil
    end

    def next_set(x)
        @next = x
    end

    def next()
        @next
    end

    def prev()
        @prev
    end

    def prev_set(x)
        @prev = x
    end
end

class Kolekcja
    attr_reader :head, :end, :length, :pointer
    def initialize(arr)
        @length = arr.length
        @head = Element.new(arr[0])
        @end = @head
        @pointer = [0,0]
        @pointer[0] = @head
        (1..@length-1).each {|x| begin @end.next_set(Element.new(arr[x])); @end.next.prev_set(@end); @end = @end.next end}
    end

    def to_s
        @a = @head
        (1..length).each {begin print @a.val, " "; @a = @a.next end}
        print("\n")
    end

    def get(i)
        if 0 <= i && i < @length
            if @pointer[1] < i
                until @pointer[1] == i do
                    @pointer[1] += 1
                    @pointer[0] = @pointer[0].next
                end
            elsif @pointer[1] > i
                until @pointer[1] == i do
                    @pointer[1] -= 1
                    @pointer[0] = @pointer[0].prev
                end
            end
            return @pointer[0].val
        end
    end

    def add(x)
        @end.next_set(new.Element(x))
        @end.next.prev_set(@end)
        @end = @end.next
    end

end


class Wyszukiwanie

    def initialize()
    end

    def szukaj1(kolekcja, v)
        def wyszukiwanie_bin(kolekcja,a,b,v)
            if a > b
                return "Nie znaleziono elementu"
            end
            s = (a+b)/2
            if kolekcja.get(s) == v
                return s
            elsif kolekcja.get(s) > v
                wyszukiwanie_bin(kolekcja,a,s-1,v)
            else
                wyszukiwanie_bin(kolekcja,s+1,b,v)
            end
        end
        if kolekcja.length > 0
            wyszukiwanie_bin(kolekcja,0,kolekcja.length-1,v)
        end
    end

    def szukaj2(kolekcja, v)
        def wysz_interpolacyjne(kolekcja,a,b,v)

            if a == b && kolekcja.get(a) == v
                return a
            elsif a >= b
                return "Nie znaleziono elementu"
            end
            s = a + ((b-kolekcja.get(a))*(b-a))/(kolekcja.get(b)-kolekcja.get(a))
            if kolekcja.get(s) == v
                return s
            elsif kolekcja.get(s) > v
                wysz_interpolacyjne(kolekcja,a,s-1,v)
            else
                wysz_interpolacyjne(kolekcja,s+1,b,v)
            end
        end
        if kolekcja.length > 0
            wysz_interpolacyjne(kolekcja,0,kolekcja.length-1,v)
        end
    end

end


kol1 = Kolekcja.new((1..1000).to_a)
kol2 = Kolekcja.new((65..90).to_a.map {|x| x.chr})

wyszukaj = Wyszukiwanie.new()
puts wyszukaj.szukaj1(kol1,2)
puts wyszukaj.szukaj2(kol1,838)
puts wyszukaj.szukaj2(kol1,1001)
puts wyszukaj.szukaj1(kol2,"X")
puts wyszukaj.szukaj1(kol2,".")