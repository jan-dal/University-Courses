using Polynomials, Printf

function solvequadratic(a, b, c)
    Δ = b^2 - 4*a*c

    if Δ == 0
        return -b/2a
    elseif Δ > 0
        Δ = sqrt(Δ)
        x0 = (-b-Δ)/2a
        x1 = (-b+Δ)/2a
        return x0, x1
    else
        Δ = 1im*sqrt(abs(Δ))
        x0 = (-b-Δ)/2a
        x1 = (-b+Δ)/2a
        return x0, x1
    end
end

function solvelinear(a, b)
    return -b/a
end


function bairstow(p, maxiter=10, precision=128)

    setprecision(precision)
    roots = []
    n = length(p)
    if n == 3
        return solvequadratic(p[3],p[2],p[1])
    elseif n == 2
        return solvelinear(p[2], p[1])
    elseif n < 2
        return []
    end

    u, v = 0.1, 0.1
    b = zeros(Float64, size(p))
    c = zeros(Float64, size(p))
    b[n] = p[n]
    c[n] = 0
    c[n-1] = p[n]
    for i in 1:maxiter
        b[n-1] = p[n-1] + u*b[n]
        for k in n-2:-1:1
            b[k] = p[k] + u*b[k+1] + v*b[k+2]
            c[k] = b[k+1] + u*c[k+1] + v*c[k+2]
        end
        J = c[1]*c[3] - c[2]^2
        u = u + (c[2]*b[2] - c[3]*b[1])/J
        v = v + (c[2]*b[1] - c[1]*b[2])/J
    end

    append!(roots,solvequadratic(1, -u, -v))
    println(u," ",v)
    append!(roots, bairstow(b[3:end]))
    return roots
end

function compare(a, b)
    @printf("Bairstow method:%sLibrary method:\n", " "^37)
    for i in 1:length(a)
        @printf("root%d = ",i)
        print(a[i], "   ",b[i])
        println("   Δ =", abs(a[i]-b[i]))
    end
end

p = Polynomial([1,2,3,4,5])
compare(bairstow([1,2,3,4,5]), roots(p))