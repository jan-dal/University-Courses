using Printf

function ieee754(x::Float64, msg::String)
    bin = bitstring(x)
    s = bin[1]
    c = bin[2:12]
    m = bin[13:end]

    @printf("%s : %s|%s|%s\n",msg,s,c,m)
end

function f(x1)

    for i in 1:1000
        x2 = nextfloat(x1)
        op1 = 1.0/x1
        op2 = 1.0/x2
        if op1 == op2
            return x1
        else
            x1 = x2
        end
    end
end

function floatxor(fl1, fl2)
    index = 0
    result = ""
    for i in bitstring(fl1)
        if i != bitstring(fl2)[1+index]
            result = string(result, "1")
        else
            result = string(result, "0")
        end
        index += 1
    end
    result = string(result[1],"|",result[2:12], "|", result[13:end])
    return result
end

function prettyprint(x, op)
    @printf("x = %.16f     1.0/x = %.16f      x(1.0/x) = %.16f\n", x, 1.0/x, op)
    @printf("x = %.16f     1.0/x = %.16f      x(1.0/x) = %.16f\n", x, BigFloat(1.0)/BigFloat(x), BigFloat(x)*(BigFloat(1.0)/BigFloat(x)))
    ieee754(1.0/x, "1.0/result")
    ieee754(x, "    result")
end

function search()
    p = rand(1.0:eps():2.0, 1)[end]
    result1 = f(p)
    if typeof(result1)<:AbstractFloat
        op1 = result1*(1.0/result1)
        prettyprint(result1, op1)
        while true
            r = nextfloat(result1)
            op2 = r*(1.0/r)
            if 1.0/result1 == 1.0/r
                prettyprint(r, op2)
                result1 = r
                op1 = result1*(1.0/result1)
            else
                break
            end
        end
    else
        search()
    end
end
for i in 1:4
    println("-"^60)
    search()
end
