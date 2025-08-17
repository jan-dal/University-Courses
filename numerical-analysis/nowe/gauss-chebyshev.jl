include("horner.jl");
using Plots;

function GaussCheb(f, n)
    xs = cos.((1:2:(2n+1))*pi/(2n+2));
    ys = f.(xs);
    return pi/(n+1)*sum(ys)
end

function GaussChebPoly(f, n, X)
    return horner(f, n, X);
end


function PlotGaussCheb(f, a, b, n;plt=false)
    h = (b-a)/n;
    xs = cos.((1:2:(2n+1))*pi/(2n+2));
    if plt == false
        plt = plot(f, a, b, fillrange = 0, fillalpha = 0.2, fillcolor = :red);
    end
    plot!(plt, GaussChebPoly(f, n, xs), a, b, fillrange = 0, fillalpha = 0.2, fillcolor = :green, label="Interpolacja",
            title=string("Kwadratura Gaussa-Czebyszewa n = ", string(n)));
    scatter!(xs, [map(f, xs)], label="Węzły");
    return plt;
end

p(x) = sqrt(1-x*x);
f(x) = sin(10*x)*cos(20*x);
g(x) = p(x)*f(x);
n = 35;

display(PlotGaussCheb(f, -1, 1, n));
