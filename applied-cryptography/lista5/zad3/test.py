import secrets
import gmpy2
import matplotlib.pyplot as plt
from gmpy2 import mpq, sqrt, invert, powmod, next_prime, gcd, invert
from cfrac import *

gmpy2.get_context().precision <<= 9
gmpy2.get_context().rational_division = True

def solve_quadratic(n, c):
    b = n - c + 1
    sd = sqrt(b*b - (n << 2))
    return (b-sd)/2, (sd+b)/2

def break_rsa(e, n):
    cf = ContFrac(mpq(e, n), 2000)
    s = 0
    for p, q in cf:
        s ^= 1
        if s or q % 2 == 0:
            continue
        c = (e*q - 1) / p
        if c == mpz(c):
            x1, x2 = solve_quadratic(n, mpz(c))
            if x1 * x2 == n:
                return x2, x1, True
    return -1, -1, False

def draw_numbers(i, bits=512, dbits=253): 
    p, q, d = 0, 0, 0
    while(p.bit_length() != bits or q.bit_length() != bits):
        p = mpz((1 << bits-1) | secrets.randbits(bits))
        q = mpz((1 << bits-1) | secrets.randbits(bits))
    
        p = next_prime(p)
        q = next_prime(q)

    pq = (p-1)*(q-1)
    while(gcd(pq, d) != 1):
        d = mpz((1 << dbits+i-1) | secrets.randbits(253+i))
    
    return p, q, d

def test(t=10000, N=15):
    s = [0 for i in range(N)]
    for i in range(N):
        for j in range(t):
            p, q, d = draw_numbers(i)
            n = p*q
            e = invert(d, n - p - q +1)
            
            p1, q1, b = break_rsa(e, n)
        
            if b: s[i] += 1
    print(s) 
    s = list(map(lambda x: x.bit_length(), s))
    print(s)

    plt.bar(range(N), s, align='center')
    plt.xlabel("i")
    plt.ylabel("-log2(Pi)")
    plt.xticks(range(N))
    plt.yticks(range(max(s)+1))
    plt.show()
    return s

test()
