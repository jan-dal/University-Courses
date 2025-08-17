using Printf
Base.promote_rule(::Type{Float64}, ::Type{Float16}) = Float16
Base.promote_rule(::Type{Int64}, ::Type{Float16}) = Float16
Base.promote_rule(::Type{Float64}, ::Type{Float32}) = Float32
Base.promote_rule(::Type{Int64}, ::Type{Float32}) = Float32

function f1(x)
    return x^3 - 6x^2 + 3x - 0.149
end

function f2(x)
    return ((x - 6)x + 3)x - 0.149
end

function calculate(f, x, exact)
    types = [Float16, Float32, Float64]
    println("-"^60)
    for i in types
        result = f(convert(i, x))
        delta = abs(exact - Float64(result))
        @printf("Arithmetic type : %s   %s(%.2f) = %f", typeof(result), f, x, result)
        println("   Error : ",delta)
    end
end

calculate(f1, 4.71, -14.636489)
calculate(f2, 4.71, -14.636489)