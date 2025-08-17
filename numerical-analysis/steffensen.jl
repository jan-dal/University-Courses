using Printf
function steffensen(f ,x0, tolerance=1.0e-6 ,maxiter=20, precision=128)
    setprecision(precision)
    xiter = x0
    for i in 1:maxiter
        print(xiter, " Δ = ")
        a = f(xiter)
        b = f(xiter + a)
        if abs(a-b) == 0.0 || xiter < 0
            break
        end
        newx = xiter - a^2/(b-a)
        Δ = abs((newx - xiter)/xiter)
        println(Δ)
        if Δ < tolerance
            break
        else
            xiter = newx
        end
    end

    return xiter
end

f(x) = -sin(x) + ℯ^(-x)

for i in 1:100
    @printf("==========================\nx0 = %d\n", i)
    println(steffensen(f, i))
end