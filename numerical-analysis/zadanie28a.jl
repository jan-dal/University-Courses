using Printf
function P(n)
    if n == 0
        return 1
    else
        return n*P(n-1)+1
    end
end


for i in 1:100
    @printf("%d ",i)
    println(P(i))
end