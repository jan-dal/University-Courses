
function test(V)
    B = zeros(length(V))
    for i in 1:length(V)
        B[i] = V[i]
    end
end

@time test([i for i=1:10000000])
