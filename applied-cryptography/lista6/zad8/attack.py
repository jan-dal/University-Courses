import secrets
from gmpy2 import mpz, powmod, invert, c_div
from Crypto.PublicKey import RSA
from oracle import *

key = RSA.generate(1024)
n = mpz(key.n)
e = mpz(key.e)
d = mpz(key.d)

oracle = Oracle(key, mpz(n) // (1 << 1))
m = mpz(int.from_bytes("Some message that we should be able to decrypt with the orcale.".encode()))
c = powmod(m, e, n)

def find_s():
    while(True):
        s  = mpz(secrets.randbelow(n))
        ci = powmod(s, e, n)*c % n
        if (oracle.tell(ci)):
            return s
""" y > x
find largest k such that x*m - k*y*m > 0 

m is unknown so to compare: x*m, k*y*m
ask the oracle about E(y - k*x)*E(m) => E(y*m - k*x*m) 
"""

def find_k(y, x):
    a = 1
    b = 1
    while(True):
        t = powmod(y - b*x, e, n)*c % n
        if oracle.tell(t):
            a, b = b, b << 1
        else:
            break

    while a < b:
        mid = c_div(a+b, 2)
        t = powmod(y - mid*x, e, n)*c % n
        if oracle.tell(t):
            a = mid
        else:
            b = mid-1
    return a

""" a > b
Find gcd(m1, m2) where:
    m1 = s1*m mod m
    m2 = s2*m mod m

m is unknown but to calculate: r = m1 - k*m2 do:
    E(r) = E(m1 - m2*k) = E(s1 - s2*k)*E(m)
    where parameters to find_k become: x = s1 - s2*k and y = x_prev
"""
def gcd(a, b):
    x0, x1 = 1, 0
    y0, y1 = 0, 1
    while b != 0:
        k = find_k(a, b)
        a, b = b, (a - k*b) % n
        #print(powmod(powmod(b, e, n)*c % n, d, n))
        x0, x1 = x1, (x0 - k*x1 % n)
        y0, y1 = y1, (y0 - k*y1 % n)

    return powmod(a, e, n)*c % n, x0, y0

dj = 0
while dj != 1:
    print("Try decrypt")
    s1 = find_s()
    s2 = find_s()
    if oracle.tell(powmod(s2 - s1, e, n)*c % n):
        s1, s2 = s2, s1

    k = find_k(s2, s1)

    m1 = s1*m % n
    m2 = s2*m % n

    dj, x, y = gcd(s1, s2)

j = s1*x + s2*y
j1 = int(invert(j, n))
print(int.to_bytes(j1, (j1.bit_length() + 7) // 8))

