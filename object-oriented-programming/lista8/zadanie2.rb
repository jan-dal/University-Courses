class ImageBW

    def initialize(x, y, img)
        @x = x
        @y = y
        if img.is_a? Array
            @img = img
        else
            @img = img.split("\n").map {|y| y.split("").map {|x| x.to_i}}
        end
    end

    def +(arg)
        @t = @img.map {|y| y.map {|x|  x&arg}}
        ImageBW.new(@x,@y,@t)
    end

    def *(arg)
        @t = @img.map {|y| y.map {|x|  x|arg}}
        ImageBW.new(@x,@y,@t)
    end

    def narysuj
        puts @img.map {|y| y.map {|x|  if x == 1 then "@" else "." end}.join()}
    end

    def img
        @img
    end
end

class ImageC
    @@greyscale = [" ",".",":","-","=","+","*","#","%","@"]

    def initialize(x, y, img)
        @x = x
        @y = y
        if img.is_a? Array
            @img = img
        else
            @img = img.split("\n").map {|y| y.split(";").map {|x| x.split(",").map {|v| v.to_i}}}
        end
    end

    def +(arg)
        @t = @img.map {|y| y.map {|x|  x.map {|v| v&arg}}}
        ImageC.new(@x,@y,@t)
    end

    def *(arg)
        @t = @img.map {|y| y.map {|x|  x.map {|v| v|arg}}}
        ImageC.new(@x,@y,@t)
    end

    def narysuj
        puts @img.map {|y| y.map {|x|  self.oblicz(x)}.join()}
    end

    def oblicz(arr)
        @n = 0.21*arr[0] + 0.72*arr[1] + 0.07*arr[2]
        @n.to_i
        @@greyscale[(@n/25.5).to_i]
    end

    def img
        @img
    end
end

class Main
    def initialize
        @image = $stdin.read
    end

    def image 
        @image
    end
end

t = Main.new()

im = ImageBW.new(3,3,t.image)

imn = im.+(1)

imn.img.each {|x| p x}
im.narysuj