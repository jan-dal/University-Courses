using LinearAlgebra

function my_invert(L, n)

    I = zeros(n, n)
    I[diagind(I)] .= one(1)

    for j in 1:n-1
        for i in j+1:n
            @. @views I[i,:] -= L[i,j]*I[j,:]
        end
    end

    return I
end


function test(n, cases)

    for _ in 1:cases
        L = LowerTriangular(rand(n, n))
        L[diagind(L)] .= one(1)
        @time r1 = norm(L*my_invert(L,n))
        @time r2 = norm(L*\(L,Matrix(I, n, n)))
        @show r1
        @show r2
    end
end

test(100, 1)