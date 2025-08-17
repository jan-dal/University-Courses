using Polynomials
function Horner4(p, x)
    a = (p[1]*x + p[2])*x + p[3]
    b = 2x*p[1] + p[2]
    c = p[1]
    d = 0
    for i in 4:length(p)
        d = d*x + c
        c = c*x + b
        b = b*x + a
        a = a*x + p[i]
    end
    return a, b, 2*c, 6*d
end


function d(p, x)
    p = Polynomial(p)
    dp = derivative(p)
    dpp = derivative(dp)
    dppp = derivative(dpp)
    return p(x), dp(x), dpp(x), dppp(x)
end

X = [1,2,3,4,5,0]
val = 10


println(Horner4(X, val))
println(d(reverse(X), val))