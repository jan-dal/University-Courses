include("simpson.jl");

exact = 3/5;
f(x) = cos(x)*cos(x)/ℯ^x;

function calculateIntegral(a, eps)
    m = 1;
    upperpart = 1/ℯ^a;
    result = upperpart;
    while abs(result - exact)/exact > eps
        lowerpart = SimpsonComposite(f, 0, a, m);
        result = upperpart + lowerpart;
        m *= 2;
    end
    return result, m;
end


println(calculateIntegral(10, 1.e-3));

#println(abs(SimpsonComposite(f, 0, 10, 1000) - exact)/exact);


