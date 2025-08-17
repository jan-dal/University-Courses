include("romberg.jl");

D(x) = x < sqrt(3)/2 ? sqrt(1-x*x) : 0.5; 
Φ(x) = Romberg(y->(sin(y)*sin(y)/sqrt(1+x*x+y*y)),-D(x), D(x), 1.0e-10);
I = Romberg(x->(sin(x)*sin(x)*Φ(x)), -1, 3, 1.0e-10);

println(I);