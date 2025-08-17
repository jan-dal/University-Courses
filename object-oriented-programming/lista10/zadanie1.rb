class Collection
    attr_reader :start, :end

    def initialize(coll)
        if coll.is_a?(Array) 
            @tab = coll 
            @start = 0
            @end = coll.length-1
        else 
            raise "Must be and array" 
        end
    end

    def length()
        return @tab.length
    end

    def get(i)
        @tab[i]
    end

    def swap(i,j)
        @tab[i],@tab[j] = @tab[j],@tab[i]
    end

    def add(e)
        @tab.append(e)
        @end += 1
    end


    def to_s()
        p @tab
    end

end

class Sorter
    
    def initialize()
    end

    def sort1(kolekcja)
        (0..kolekcja.length-1).each {|x| (x..kolekcja.length-1).each {|y| kolekcja.swap(x,y) if kolekcja.get(x) > kolekcja.get(y)}}
    end

    def sort2(kolekcja) # Szybsza metoda
        def partition(kolekcja,l,h)
            pivot = kolekcja.get(h)
            i = l - 1
            for j in (l..h-1) do
                if kolekcja.get(j) < pivot
                    i += 1
                    kolekcja.swap(i,j)
                end
            end
            kolekcja.swap(i+1,h)
            return i+1
        end
        def quicksort(kolekcja,i,j)
            if i < j
                pi = partition(kolekcja,i,j)
                quicksort(kolekcja,i,pi-1)
                quicksort(kolekcja)
            end
        end
        quicksort(kolekcja,kolekcja.start,kolekcja.end)
    end

end
num = Collection.new([5,7,3,1,1,3,5,4,8,1,4,1,10,3])

num2 = Collection.new((1..100).to_a.map {|x| 101-x})
letters = Collection.new(["a","t","c","aa","q","ax"])

sort = Sorter.new()
sort.sort2(num2)
sort.sort1(num)
sort.sort2(letters)
num2.to_s
num.to_s
letters.to_s
