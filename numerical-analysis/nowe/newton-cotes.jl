using Polynomials;
include("horner.jl")

### START --- Newnton-Cotes

function Interpolation_A(a, b, k, n)
    h = (b-a)/n;
    ipoly = integrate(fromroots([j for j in 0:n if j!=k]));
    return h*(-1)^(n-k)*(ipoly(n) - ipoly(0))/(factorial(big(k))*factorial(big(n-k)));
end

function NewtonCotes(f, a, b, n)
    h = (b-a)/n;
    return sum([Interpolation_A(a, b, k, n)*f(a+k*h) for k in 0:n])
end

function NewtonCotesPoly(f, X, n)
    return horner(f, n, X);
end

function NewtonCotesCompositePoly(f, a, b, m, n)
    h = (b-a)/m;
    hp = h/n;
    TPol = [NewtonCotesPoly(f, [a+i*h + k*hp for k in 0:n], n) for i in 0:m];
    return x->(TPol[Int64(floor((x-a)/h)+1)](x));
end

# n - stopień
# m - obszary

function PlotNewtonCotes(f, a, b, n, m;plt=false)
    h = (b-a)/m;
    hp = h/n;
    outer_points = [a + k*h for k in 0:m];
    inner_points = [outer_points[k] + i*hp for i in 1:n-1 for k in 1:m];
    if plt == false
        plt = plot(f, a, b, fillrange = 0, fillalpha = 0.2, fillcolor = :red);
    end
    plot!(plt, NewtonCotesCompositePoly(f, a, b, m, n), a, b, label = "Interpolacja", fillrange = 0, fillalpha = 0.2, fillcolor = :green,
            title=string("Newton-Cotes n = ", string(n)))
    scatter!(plt, outer_points, map(f, outer_points), label = "Węzły zewn.", markersize=5);
    scatter!(plt, inner_points, map(f, inner_points), label = "Węzły wewn.", markersize=4);
    return plt;
end

### END --- Newton-Cotes
