using Plots
include("horner.jl");

function TrapezoidPoly(f, X)
    return horner(f, 1, X)
end

function TrapezoidCompositePoly(f, a, b, n)
    h = (b-a)/n;
    TPol = [TrapezoidPoly(f, [a + i*h, a + (i+1)*h]) for i in 0:n];
    return x->(TPol[Int64(floor((x-a)/h)+1)](x));
end

function Trapezoid(f, a, b)
    return (f(b)+f(a))/2
end

function TrapezoidComposite(f, a, b, n)
    h = (b-a)/n;
    return h*(1/2*(f(a)+f(b))+sum(map(f, (a+h):h:(b-h))))
end

function TrapezoidRest(a, b)
    return x->-(b-a)^3/12*x
end

function TrapezoidCompositeRest(a, b, n)
    h = (b-a)/n;
    return x->x*(a-b)*h*h/12;
end

function PlotTrapezoid(f, a, b, n; plt=false)
    h = (b-a)/n;
    if plt == false
        plt = plot(f, a, b, fillrange = 0, fillalpha = 0.2, fillcolor = :red);
    end
    plot!(plt, TrapezoidCompositePoly(f, a, b, n), a, b, fillrange = 0, fillalpha = 0.2, fillcolor = :green, label="Interpolacja",
            title=string("Złożony wzór trapezów n = ", string(n)));
    scatter!([a:h:b], [map(f, a:h:b)], label="Węzły");
    return plt;
end

PlotTrapezoid(x->1.0/(1.0+x^4), 0, 1, 2);
