include("newton-cotes.jl");

f(x) = 1/(1+x*x);
ex = 2*atan(4);

for i in 2:2:10
    Q = NewtonCotes(f, -4, 4, i);
    @printf("Q%d(f) = %.8f R = %e \n", i, Q, abs(ex-Q));
    #display(PlotNewtonCotes(f, -4, 4, i, 1));
end
