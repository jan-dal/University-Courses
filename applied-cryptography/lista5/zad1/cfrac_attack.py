from gmpy2 import mpq, iroot
from cfrac import *

n = 1966981193543797
e = 323815174542919

cf = ContFrac(mpq(e, n))

def solve_quadratic(n, c):
    b = n - c + 1
    sd, t = iroot(b*b - (n << 2), 2)
    if not t:
        return 0, 0
    
    return (b - sd) >> 1, sd+b >> 1

def break_rsa(e, n):

    cf = ContFrac(mpq(e, n))
    s = 0
    for p, q in cf:
        print(f"{p} / {q}")
        s ^= 1
        if s or q % 2 == 0:
            continue
        c = (e*q - 1) / p
        if c == mpz(c):
            x1, x2 = solve_quadratic(n, mpz(c))
            if x1 * x2 == n:
                break

    return x2, x1
        
    
print(break_rsa(e, n))
