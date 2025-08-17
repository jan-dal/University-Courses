from gmpy2 import mpz, invert, powmod
from cryptography.hazmat.primitives.asymmetric import rsa
from time import time
import os

e = mpz(65537)

def test1(c, p, q):
    n = p*q
    pq1 = n - p - q + 1
    e1 = invert(e, pq1)
    d  = e1 % pq1 
    return powmod(c, d, n)

def test2(c, p, q):
    pq1 = (p-1)*(q-1)
    e1 = invert(e, pq1)
    dp = e1 % (p-1)
    dq = e1 % (q-1)
    mp = powmod(c, dp, p)
    mq = powmod(c, dq, q)
    
    q1 = invert(q, p)
    qinv = q1 % p
    h = (qinv*(mp - mq)) % p
    return mq + h*q

t1acc = 0
t2acc = 0
N = 10

for i in range(N):
    pk = rsa.generate_private_key(65537, 4096)
    p = mpz(pk.private_numbers().p)
    q = mpz(pk.private_numbers().q)
    c = mpz(int.from_bytes(os.urandom(512)))

    t1 = time()    
    m1 = test1(c, p, q)
    t1acc += time() - t1

    t2 = time()
    m2 = test2(c, p, q)
    t2acc += time() - t2

    assert m1 == m2

print(f"Average standard method time: {t1acc / N * 1000:.3f}ms")
print(f"Average improved method time: {t2acc / N * 1000:.3f}ms")
