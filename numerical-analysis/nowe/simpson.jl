using Plots
include("horner.jl");

function SumBis(f, a, b, m)
    h = (b-a)/m;
    return h*(0.5*(f(a)+f(b))+sum(map(f, [a+k*h for k in 1:m-1])))
end

function SimpsonPoly(f, X)
    return horner(f, 2, X);
end

function SimpsonCompositePoly(f, a, b, m)
    h = (b-a)/m;
    TPol = [SimpsonPoly(f, [a + i*h, a + i*h + h/2, a + (i+1)*h]) for i in 0:m];
    return x->(TPol[Int64(floor((x-a)/h)+1)](x));
end

function Simpson(f, a, b)
    return (b-a)*(f(a)+4*f((a+b)/2)+f(b))/6
end

function SimpsonComposite(f, a, b, m)
    return (4*SumBis(f, a, b, 2*m)-SumBis(f, a, b, m))/3;
end

function SimpsonRest(a, b)
    h = (b-a)/2;
    return x->-h^5/90*x
end

function SimpsonCompositeRest(a, b, n)
    h = (b-a)/n;
    return x->x*(a-b)*h^4/180;
end

function PlotSimpson(f, a, b, m; plt=false)
    h = (b-a)/m;
    if plt == false
        plt = plot(f, a, b, fillrange = 0, fillalpha = 0.2, fillcolor = :red);
    end
    plot!(plt, SimpsonCompositePoly(f, a, b, m), a, b, fillrange = 0, fillalpha = 0.2, fillcolor = :green, label="Interpolacja",
        title=string("Złożony wzór simpsona n = ", string(m*2)));
    scatter!(plt, [a:h:b], [map(f, a:h:b)], label="Węzły zewn.");
    scatter!(plt, [a+(h/2):h:b], [map(f, a+(h/2):h:b)], markersize=3, label="Węzły wewn.")
    return plt;
end
