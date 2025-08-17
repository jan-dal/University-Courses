from gmpy2 import t_divmod
from RM import *

primes = []
for p in open("primes.txt"):
    primes.append(int(p))

def test_by_division(x):
    for p in primes:
        if p >= x:
            break
        if x % p == 0:
            return False
    return True


def next_prime(x):
    if x <= 2:
        return mpz(2)

    x = mpz(x)
    s = 0
    if x % 2 == 0: 
        x += 1
        s += 1
    found = False

    while(not found):
        no_small_div = test_by_division(x)
        if no_small_div:
            found = not rabin_miller(x, repeat=25)
            if found: break

        x += 2
        s += 2
    return x

def ext_euclidean(a, b):
    x0, x1, y0, y1 = 0, 1, 1, 0
    while(a != 0):
        (q, a), b = divmod(b, a), a
        y0, y1 = y1, y0 - q*y1
        x0, x1 = x1, x0 - q*x1
    return b, x0, y0

def inverse(a, m):
    r, x, _ = ext_euclidean(a, m)
    assert r == 1, "No inverse exists!"
    return x % m

