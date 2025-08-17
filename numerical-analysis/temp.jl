using Printf
function newton( f::Function, df::Function, x0; ɛs=1.0e-6, imax=20, print=false )
    iter = 0; 
    xr = x0; 
    ɛa = one(x0)
    if print; @printf("Iter %2d : xr=%10.7f  ɛa=%.3e\n" , iter, xr, ɛa); end;
    while true
        xrold = xr
        iter  = iter+1
        xr    = xrold - f(xrold)/df(xrold)
        if xr!=0.0
            ɛa = abs((xr-xrold)/xr)
        end;
        if print; @printf("Iter %2d : xr=%10.7f  ɛa=%.3e\n" , iter, xr, ɛa); end;
        if ɛa<ɛs || iter>=imax
            break;
        end
    end
    return xr,iter,ɛa;
end;

function regulafalsi( f::Function, x0, x1; ɛs=1.0e-6, imax=20, print=false )
    iter = 0; 
    xn1 = x1;
    xnp = x0;
    ɛa = one(x0)
    xs = []
    if print; @printf("Iter %2d : xn1=%10.7f xnp=%10.7f  ɛa=%.3e\n" , iter, xn1, xnp, ɛa); end;
    while true
        for i in xs
            if f(xn1)*f(xnp) < 0
                xnp = i
            end
        end
        iter = iter+1
        newx = xn1 - (f(xn1)*(xn1-xnp))/(f(xn1)-f(xnp))
        append!(xs, newx)
        if xn1!=0.0
            ɛa = abs((newx-xn1)/newx)
        end;
        xn1 = newx
        if print; @printf("Iter %2d : xn1=%10.7f xnp=%10.7f  ɛa=%.3e\n" , iter, xn1, xnp, ɛa); end;
        if ɛa<ɛs || iter>=imax
            break;
        end
    end
    return xn1,iter,ɛa;
end;



function pierwiastek(R, x)

    for i in 0:20
        xold = x
        x = x*(x^2+3R)/(3*x^2+R)
        @printf("x = %.16f  e = %.16f \n",x, abs((xold-x)/x))
    end
    println(x)
    return x
end

function odwrotnosc(c, x)

    for i in 0:20
        println(x - (1/c))
        xold = x
        x = x*(2-c*x)
    end
    println(x)
end    

f(x) = e^x
odwrotnosc(2,0.1)