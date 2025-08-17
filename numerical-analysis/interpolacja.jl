using Polynomials

f(x) = ℯ^(atan(x))

function T(n)
    if n == 0
        return Polynomial([1])
    elseif n == 1
        return Polynomial([0,1])
    else
        return 2*Polynomial([0,1])*T(n-1) + T(n-2)
    end
end

function uj(j,n, a, b)
    p = cos(j*pi/n)

    return (b-a)/2*p + (a+b)/2
end

function B(k, n, a, b)
    suma = 0
    for j in 0:n
        w = uj(j, n, a, b)
        if (j == 0) || (j == n)
            suma += 1/2*f(w)*(T(k))(w)
        else
            suma += f(w)*(T(k))(w)
        end
    end
    return 2/n*suma
end

function J(n, a, b)
    result = Polynomial([0])
    for i in 0:n
        if i == 0 || i == n
            result += 1/2*B(i, n, a, b)*T(i)
        else
            result += B(i, n, a, b)*T(i)
        end
    end
    return result
end


