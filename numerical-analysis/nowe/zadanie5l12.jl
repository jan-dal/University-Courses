using Plots

function Euler(yn, h, f, a,k=0)
    ynew = yn + h*f(a);
    for i in 1:k
        ynew = ynew + h*f(a);
    end
    return ynew;
end

function trajectory(κ, x0, y0, u0, v0, h=1.e-6)
    z = κ*sqrt(u0*u0 + v0*v0);
    un = Euler(u0, h, (x)->(-z*x), u0);
    vn = Euler(v0, h, (x)->-(9.81+(z*x)), v0);
    xn = Euler(x0, h, (x)->x, u0);
    yn = Euler(y0, h, (x)->x, v0);
    ystart = y0;
    X, Y = [xn], [yn];

    while ystart < yn;
        z = κ*sqrt(un*un + vn*vn);
        un = Euler(un, h, (x)->(-z*x), un);
        vn = Euler(vn, h, (x)->-(9.81+(z*x)), vn);
        xn = Euler(xn, h, (x)->x, un);
        yn = Euler(yn, h, (x)->x, vn);
        X = vcat(X, [xn]);
        Y = vcat(Y, [yn]);
    end

    return X, Y;
end

Θ = pi/3;
x0, y0 = 0, 0;
#u0, v0 = 100*cos(Θ), 100*sin(Θ);
κ = 0.05;
plt = plot();

u0, v0 = 0.2*cos(Θ), 0.2*sin(Θ);
plot!(plt, trajectory(κ, x0, y0, u0, v0), label=κ);

display(plt);