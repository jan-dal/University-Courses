using Printf

function TrapezoidComposite(f, a, b, n)
    h = (b-a)/n;
    return h*(0.5*(f(a)+f(b))+sum(map(f, (a+h):h:(b-h))))
end

function PrecisionTrapezoid(f, a, b, exact, eps)
    n = 1;
    result = TrapezoidComposite(f, a, b, n);
    while abs(result - exact)/abs(exact) > eps
        n += 1;
        result = TrapezoidComposite(f, a, b, n);
    end
    return result, n;
end

function RombergTable(f, a, b, eps=1.0e-8)
    table = [[TrapezoidComposite(f, a, b, 2^i) for i in 0:1]];
    append!(table, [[(4*table[1][2] - table[1][1])/3]]);
    k, i = 2, 2;
    while abs(table[k][1] - table[k-1][1]) >= eps*abs(table[k][1])
        append!(table[1], TrapezoidComposite(f, a, b, 2^i));
        i += 1;
        for m in 1:k-1
            append!(table[m+1], (4^m*table[m][k+2-m] - table[m][k+1-m])/(4^m-1));
        end
        append!(table, [[(4^k*table[k][2] - table[k][1])/(4^k-1)]]);
        k += 1;
    end
    return table;
end

function Romberg(f, a, b, eps=1.0e-8)
    if (a < b)
        return RombergTable(f, a, b, eps)[end][end];
    else
        return 0;
    end
end

function PrintRombergTable(T, exact)
    n = length(T)
    sortT = [[T[i][j] for i in 1:n-j+1] for j in 1:n ];

    @printf("Table values:\n\n");

    for i in 1:n
        @printf("%d. ", i);
        for j in 1:n-i+1
            @printf("| %.8e ", sortT[i][j]);
        end
        @printf("|\n");
    end

    @printf("\nRelative error:\n\n");

    for i in 1:n
        @printf("%d. ", i);
        for j in 1:n-i+1
            @printf("| %.8e ", abs((exact - sortT[i][j])/exact));
        end
        @printf("|\n");
    end
end