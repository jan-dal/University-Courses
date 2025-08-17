import secrets
import gmpy2
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
