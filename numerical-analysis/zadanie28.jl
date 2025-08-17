using Printf


function C(k)
    if k == 2
        return 0
    end
    c_kprev = 0
    i = 2
    while true
        if i == k
            break
        end
        c_knext = sqrt(0.5+(c_kprev/2))
        c_kprev = c_knext
        i += 1
    end
    return c_kprev
end

function C_p(k)
    if k == 2
        return 0
    end
    x = C_p(k-1)
    return (1/16)*(-x^2+5x+(23/4))
end

function S(k)
    if k == 2
        return 1
    end
    return sqrt(0.5(1-C(k-1)))
end

function P(k)
    if k == 4
        return 2
    end
    return 2^(k-1)*S(k)
end


for i in 2:60
    @printf("%d %.16f   ", i, C(i))
    @printf("%d %.16f   \n", i, C_p(i))
end
