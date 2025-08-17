using HTTP, Printf;
include("trapezoid.jl");
include("simpson.jl");
include("newton-cotes.jl");
#hdf5();

f4 = 1.61e5;
function UnknownFunction(x)
    url = string("http://roxy.pythonanywhere.com/f3?x=", string(x));
    r = HTTP.request("GET", url);
    return parse(Float64, String(r.body))
end

#= #Zapisuje wykres nieznanej funkcji w pamięci
p = plot(UnknownFunction, 0, 1, label="f", fillrange = 0, fillalpha = 0.2, fillcolor = :red, legend=:topleft);
Plots.hdf5plot_write(p, "ufunction.hdf5");
display(p);
=#

## Obliczenie funkcji dla n = 1, 2 metodą Netona-Cotesa

R1 = TrapezoidRest(0, 1);
R2 = SimpsonRest(0, 1);

@printf("Newton-Cotes n = %d, Q = %.16f \n", 1, NewtonCotes(UnknownFunction, 0, 1, 1));
@printf("Maksymalny błąd R = %f\n", R1(f4));



@printf("Newton-Cotes n = %d, Q = %.16f \n", 2, NewtonCotes(UnknownFunction, 0, 1, 2));
@printf("Maksymalny błąd R = %f\n", R2(f4));

## Obliczenie funckji złożonymi wzorami trapezów oraz simpsonem dla n = 2, 4, 8, 16, 32

for i in 1:5
    n = 2^i;
    R = TrapezoidCompositeRest(0, 1, n);
    @printf("Złożony wzór trapezów n = %d, Q = %.16f \n", n, TrapezoidComposite(UnknownFunction, 0, 1, n));
    @printf("Maksymalny błąd R = %f\n", R(f4));
end

for i in 0:4
    m = 2^i;
    R = SimpsonCompositeRest(0, 1, 2*m);
    @printf("Złożony simpson n = %d, Q = %.16f \n", m*2, SimpsonComposite(UnknownFunction, 0, 1, m));
    @printf("Maksymalny błąd R = %f\n", R(f4));
end

#= ## Wykresy
gr();

plt = Plots.hdf5plot_read("ufunction.hdf5")
p = PlotNewtonCotes(UnknownFunction, 0, 1, 1, 1, plt=plt);
png(p, string("NewtonCotes", string(1)));

plt = Plots.hdf5plot_read("ufunction.hdf5")
p = PlotNewtonCotes(UnknownFunction, 0, 1, 2, 1, plt=plt);
png(p, string("NewtonCotes", string(2)));

for i in 1:5
    n = 2^i;
    plt = Plots.hdf5plot_read("ufunction.hdf5")
    p = PlotTrapezoid(UnknownFunction, 0, 1, n, plt=plt);
    png(p, string("TrapezoidComposite",string(n)));
end
for i in 0:4
    n = 2^i;
    plt = Plots.hdf5plot_read("ufunction.hdf5")
    p = PlotSimpson(UnknownFunction, 0, 1, n, plt=plt);
    png(p, string("SimpsonComposite",string(n*2)));
end
=#
