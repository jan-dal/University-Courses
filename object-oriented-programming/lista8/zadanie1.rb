class Integer
    def czynniki
        if self >= 0
            (1..self).select {|x| (self % x) == 0}
        else
            (1..-self).select {|x| (self % x) == 0}
        end
    end

    def ack(y)
        if self == 0
            y + 1
        elsif y == 0
            (self-1).ack(1)
        else
            (self-1).ack(self.ack(y-1))
        end
    end

    def doskonala
        if self >= 0
            (self.czynniki.sum-self) == self
        else
            false
        end
    end

    def slownie
        self.to_s.split("").map {|x| if x == "-" then "minus" else x.to_i.nazwij end}.join(" ")
    end
    
    protected
    def nazwij
        case self
        when 0
            "zero"
        when 1
            "jeden"
        when 2
            "dwa"
        when 3
            "trzy"
        when 4
            "cztery"
        when 5
            "pięć"
        when 6
            "sześć"
        when 7
            "siedem"
        when 8
            "osiem"
        when 9
            "dziewięć"
        end
    end
end
p 10.czynniki
p 0.czynniki
p 1000.czynniki
puts 2.ack(1)
puts 3.ack(8)
puts 6.doskonala
puts 6.slownie
puts 56.doskonala
puts 78123.slownie
puts 8128.doskonala
puts 8128.slownie
puts -10.slownie