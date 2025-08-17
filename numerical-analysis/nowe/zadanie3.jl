include("simpson.jl");
include("trapezoid.jl");

f(x) = sin(x);
exact = 2.0;

eps = 2.0e-5;


#println(abs(SimpsonComposite(f, 0, pi, 7) - exact)/exact <= eps);
#println(SimpsonComposite(f, 0, pi, 7)); # n = 14

#println(abs(TrapezoidComposite(f, 0, pi, 253) - exact)/exact <= eps);
#println(TrapezoidComposite(f, 0, pi, 253));

display(PlotSimpson(f, 0, pi, 7));
display(PlotTrapezoid(f, 0, pi, 253));