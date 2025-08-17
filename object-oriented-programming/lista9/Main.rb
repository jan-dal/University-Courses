class Function
    def initialize(f)
        @funkcja = f
    end

    def value(x)
        @funkcja.call(x)
    end
    
    def derivate(x)
        x = x.to_f
        prec = 0.00001
        x1, x2 = x-prec, x+prec
        y1,y2 = self.value(x1), self.value(x2)
        return (y2-y1) / (x2-x1)
    end
    
    def snd_derivative(x)
        x = x.to_f
        prec = 0.00001
        x1, x2 = x-prec, x+prec
        y1, y2 = self.derivate(x1), self.derivate(x2)
        return (y2-y1) / (x2-x1)
    end

    def zero(a,b,e)
        #Metoda Newtona f(a) i f(b) muszą być różnych znaków
        #Druga pochodna f(x0) musi być tego samego znaku co f(x0)
        #Pomiędzy x0, a miejscem zerowym pochodna f(x) nie może przybierać 0
        a,b = a.to_f, b.to_f
        fa, fb = self.value(a), self.value(b)
        x = rand(a..b)
        fs = self.value(x)
        for i in (1..1000) do
            if (self.snd_derivative(x)*fs > 0)
                x = rand(a..b)
            fs = self.value(x)
            else
                break
            end
        end
        
        if (fa*fb > 0)
            return nil
        elsif (fa == 0)
            return a
        elsif (fb == 0)
            return b
        end
        while (fs.abs > e) do
            x = x - fs/self.derivate(x)
            fs = self.value(x)
        end
        
        return x
    end

    def field(a,b)
        a,b = a.to_f,b.to_f
        prec = 1000
        step = (a-b).abs()/prec
        values = (0..prec-1).to_a.map {|x| self.value(a+x*step) >= 0 ? self.value(a+x*step)*step : 0}
        return values.sum
    end 

    def graph_ascii(a,b,thickness,pitch,yscale)
        #resolution 81x20
        a,b = a.to_f, b.to_f
        wykres = (1..20).map {|y| (1..81).map {|x| "."}}
        step = (a-b).abs/79
        (1..20).each {|y| (1..80).each {|x| wykres[y-1][x-1] = "@" if # (y - f(x)).abs < step*d/dxf(x)*thickness
            ((self.normalize(10-y+pitch,0,step,yscale) - self.value(self.normalize(x-1,a,step,1))).abs < self.tolerance(x,step,thickness,a,yscale))}}
        wykres[0][80] = self.normalize(9+pitch,0,step,yscale).round(3).to_s
        wykres[19][80] = self.normalize(-10+pitch,0,step,yscale).round(3).to_s
        wykres[9+pitch][80] = "0"

        (0..19).each {|x| print wykres[x].join() + "\n"}
    end
    
    def graph(a,b,thickness,pitch,yscale)
        a,b = a.to_f, b.to_f
        #resolution 1080x1920
        bitmapa = (1..1080).map {|y| (1..1920).map {|x| 255}}
        step = (a-b).abs/1919
        (1..1080).each {|y| (1..1920).each {|x| (bitmapa[y-1][x-1] = 0) if 
            ((self.normalize(540-y+pitch,0,step,yscale) - self.value(self.normalize(x-1,a,step,1))).abs < self.tolerance(x,step,thickness,a,yscale))}}
        #bitmapa = self.antialiassing(bitmapa,2)
        (0..1919).each {|x| bitmapa[540+pitch][x] = 0} #x-axis
        content = ""
        (0..1079).each {|x| content.concat(bitmapa[x].join(" ") + "\n")}
        File.write('Wykres'+ self.object_id.to_s() + a.to_s() + b.to_s() +'.pbm',"P2\n1920 1080\n" + content)
    end

    protected
    def normalize(n,a,step,yscale)
        return (a + n*step)*yscale
    end

    def tolerance(x,step,thickness,a,yscale)
        if self.derivate(self.normalize(x-1,a,step,1)).abs < 1/yscale
            return step*thickness
        else
            return step*self.derivate(self.normalize(x-1,a,step,1)).abs()*thickness
        end
    end

    def antialiassing(array,boxsize)
        new_array = array.clone
        def average(x,y,boxsize,arr)
            avg = 0
            for i in (-boxsize..boxsize) do
                for j in (-boxsize..boxsize) do
                    begin
                        avg += arr[y+i][x+j]
                    rescue

                    end
                end
            end
            return avg/(2*boxsize+1)**2.to_i
        end
        (0..1079).each {|y| (0..1919).each {|x| new_array[y][x] = average(x,y,boxsize,array)}}
        return new_array
    end

end

f = Function.new(lambda {|x| Math.sin(x)*Math.sin(x)*Math.cos(x)})
g = Function.new(lambda {|x| x*x*x})
h = Function.new(lambda {|x| Math.sin(x*x)})
i = Function.new(lambda {|x| Math.sin(x)})
j = Function.new(lambda {|x| x})
k = Function.new(lambda {|x| x*x})
o = Function.new(lambda {|x| 10})
q = Function.new(lambda {|x| Math.sin(2*Math.sin(2*Math.sin(2*Math.sin(x))))})
u = Function.new(lambda {|x| x*x*x*Math.sin(x)})

puts "Pochodne:"
puts i.derivate(0)
puts i.derivate(3.1415/2)
puts j.derivate(1)
puts j.derivate(-100)

puts "Miejsca zerowe:"
puts i.zero(2,4,0.001)
puts g.zero(-100,100,0.001)
puts j.zero(-2,200,0.01)

puts "Pola:"
puts j.field(0,1)
puts o.field(0,2)
puts i.field(-3.14,3.14)
puts i.field(0,3.14)

#f.graph_ascii(-20,20,0.5,0,0.5)
#g.graph_ascii(-1,1,1,0,1)
i.graph_ascii(-3.14,10*2.14,0.5,3,0.5)

#Alternatywa metody graph_ascii -> tworzy plik pbm z wykresem
#u.graph(-100,100,5,0,10000)
#f.graph(-7,7,1,0,3)
#g.graph(-10,2,3.5,200,1)
#i.graph(0,-3.14*3,2,0,1)
#h.graph(-20,20,1,0,3)
#j.graph(-10,10,2,300,1)
#k.graph(-10,10,2,0,1)
#o.graph(-50,50,3,0,1)
