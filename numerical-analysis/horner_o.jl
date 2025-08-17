using Plots
using Polynomials

gr();

function pprim(i, k, X)
    p = 1;
    xi = X[i];
    for j in 1:k+1;
        if j!=i
            p *= (xi - X[j]);
        end
    end
    return p;
end

function wsp_b(f, k, X)
    b = 0;
    for i in 1:k+1
        p = pprim(i, k, X);
        b += f(X[i])/p;
    end
    return b;
end


function horner(p, n, X)
    w = Polynomial([wsp_b(p,n, X)]);
    for i in n:-1:1
        f = Polynomial([-X[i], 1]);
        b = wsp_b(p, i-1, X);
        w = w*f + b;
    end
    return w;
end
X = -5:0.6:5

p(x) = 1/(x^2);
Lp = horner(p, length(X)-1, X);


plt = plot(p, -10:0.001:10);
plot!(Lp, -10:0.001:10, xlims=(-10,10), ylims=(-10,10), label="");

display(plt);