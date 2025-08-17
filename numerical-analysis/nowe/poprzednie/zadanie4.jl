include("newton-cotes.jl");
using Printf;

function I_B(a, b, k, n)
    B = Interpolation_A(a, b, k, n)/(b-a);
    #@printf("k = %d, B = %.3f \n", k, B);
    return B;
end

function σ(a, b, n)
    for i in 0:n
        println(sum([abs(I_B(a, b, k, n)) for k in 0:i]));
    end
end

a, b = 0, 10;


for n in 1:21
    println("Dla n = ", string(n));
    println(sum([I_B(a, b, k, n) for k in 0:n]));
end



for i in 10:5:20
    println("Dla n = ", string(i));
    σ(a, b, i);
end
