using LinearAlgebra, Polyester
# Eliminacja Gaussa 

function solve!(A::Matrix{Float64}, b::Vector{Float64})
    n = size(A)[1];
    A = permutedims(A, (2, 1))
    x = zeros(n)
    p = [i for i=1:n]
    maxa = A[1]
    s = zeros(n)
    u, k = 1, 1
    
    #for x in A
    #    cp = abs(x)
    #    if cp > maxa
    #        maxa = cp
    #    end
    #    if k % n == 0
    #        @inbounds s[u] = maxa
    #        try
    #            maxa = A[u*n+1]
    #        catch LoadError
    #            continue
    #        end
    #        u += 1
    #        k = 1
    #        continue
    #    end
    #    k += 1
    #end

    for k = 1:n-1
    #    @inbounds eg = abs(A[k,p[k]])/s[p[k]]
    #    j = k
    #    for i=k:n
    #        @inbounds neweg = abs(A[k,p[i]])/s[p[i]]
    #        if eg < neweg
    #            eg = neweg
    #            j = i
    #        end
    #    end
    #    t = p[k]
    #    p[k] = p[j]
    #    p[j] = t
    
        for i = k+1:n
            @inbounds z = A[k,p[i]]/A[k,p[k]]
            @inbounds A[k, p[i]] = z
            #for j = k+1:n
            #    @inbounds A[j, p[i]] -= z*A[j, p[k]] 
            #end
            
            #@inbounds @. @views A[k+1:n, p[i]] -= z*A[k+1:n, p[k]]
            @views @inbounds BLAS.axpy!(-z, A[k+1:n, p[k]], A[k+1:n, p[i]])
        end
    end

    for k = 1:n-1
       for i = k+1:n
           @inbounds b[p[i]] -= b[p[k]]*A[k, p[i]]
       end
    end
    @inbounds for i = n:-1:1
       ax = zero(0)
       for j = i+1:n
           ax += A[j, p[i]]*x[j]
       end
       @inbounds x[i] = (b[p[i]] - ax)/A[i, p[i]]
    end
    x
end

function test(n, k)
    for i = 1:n
        A = rand(k, k)
        b = rand(k)
        B = copy(A)
        c = copy(b)

        @time x1 = A \ b
        @time x2 = solve!(A, b)
        @show norm(B*x1-c)
        @show norm(B*x2-c)
    end
end
A = [2.0 3.0 -6.0; 1.0 -6.0 8.0; 3.0 -2.0 1.0]
b = [2.2, 1.0, 42.0]

test(1, 1000)
