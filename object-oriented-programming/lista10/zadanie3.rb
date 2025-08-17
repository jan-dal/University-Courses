class Snapshot
    def initialize()
    end

end

class Test
    def initialize(v)
        @v = v
    end

    def to_s
        puts v
    end
end

x = 2

y = 3

te = Test.new(4)

local_variables.each{|x| p x}
p ObjectSpace.each_object.map(&:class).uniq