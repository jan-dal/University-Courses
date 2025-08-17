using Printf

function ieee754(x::Float64)
    bin = bitstring(x)
    s = bin[1]
    c = bin[1:11]
    m = bin[12:end]

    @printf("%s|%s|%s\n",s,c,m)
end

function verbose(x::Float64)
    op1f = 1.0/x
    op1b = 1.0/BigFloat(x)
    op2 = x*op1f
    @printf("1.0/%s = %s\n", string(x),string(op1f))
    @printf("1.0/%s = %s\n", string(x),string(op1b))
    ieee754(x)
    ieee754(op1f)
    ieee754(op2)
end

function search_next(x::Float64)
    while true
        if x > 2.0
            return 2.0
        end
        x = nextfloat(x)
        if x*(1/x) != 1.0
            return x
        end
    end
end

function search(x::Integer)::Vector{Float64}
    results = zeros(0)
    for i in 1:x
        s = rand(1.0:eps():2.0, 1)
        s = search_next(s[end])
        if s < 2.0
            append!(results, s)  
        end
    end
    for i in results
        if i != 0
            @printf("---------------------------------------------------------------")
            println()
            verbose(i)
            println()
            verbose(nextfloat(i))
        end
    end
    return results
end

search(30)
