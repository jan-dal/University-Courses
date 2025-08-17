function maxabs1(V::Matrix{Float64})
    maxa = V[1]
    n = size(V)[1]
    s = zeros(n)
    k = 1
    u = 1
    for i in V
        cp = abs(i)
        if cp > maxa
            maxa = cp
        end
        if k % n == 0
            s[u] = maxa
            try
                maxa = V[u*n+1]
            catch LoadError
                continue
            end
            u += 1
            k = 1
            continue
        end
        k += 1
    end

    nothing
end

function maxabs2(V)
    n = size(V)[1]

    [maximum(abs.(V[1+j*n:n*(j+1)])) for j=0:n-1]
    nothing
end

function test(n)
    for i in 1:10
        V = rand(n)
        print("first : ")
        @time maxabs1(V)
        print("second : ")
        @time maxabs2(V)
        println()
    end
end

V = rand(10000, 10000)

@time maxabs1(V)
@time maxabs2(V)