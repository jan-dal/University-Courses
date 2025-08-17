import Crypto.PublicKey.RSA as RSA
from gmpy2 import mpz, iroot, powmod, gcd, invert

def force(c, n):
    for i in range(1000000):
        m, t = iroot(n*i+c, 3)
        if t:
            m = int(m)
            print(m.to_bytes((m.bit_length() + 7) // 8).decode(errors="ignore"))
            break

def solve_chr(A, N):
    M = 1
    nLen = len(N)
    for i in range(nLen):
        M *= N[i]
    Y = [M // N[i] for i in range(nLen)]
    Z = [invert(Y[i], N[i]) for i in range(nLen)]
    
    return sum([A[i]*Y[i]*Z[i] for i in range(nLen)]) % M

message = int.from_bytes("Some longer very secret message from Adam to Bob, Chris and  Denis. Extra padding pad pad pad pad pad pad pad pad pad pad pad .".encode())

print(f"Message lenght: {message.bit_length()}")

r1, r2, r3 = 0, 0, 0
n1, n2, n3 = 0, 0, 0
while(r1 != 1 or r2 != 1 or r3 != 1):
    k1 = RSA.generate(1024, e=3)
    k2 = RSA.generate(1024, e=3)
    k3 = RSA.generate(1024, e=3)

    n1 = mpz(k1.n)
    n2 = mpz(k2.n)
    n3 = mpz(k3.n)
    
    r1 = gcd(n1, n2)
    r2 = gcd(n2, n3)
    r3 = gcd(n3, n1)

c1 = powmod(message, 3, n1)
c2 = powmod(message, 3, n2)
c3 = powmod(message, 3, n3)


A = [c1, c2, c3]
N = [n1, n2, n3]
x = solve_chr(A, N)

assert x % n1 == c1
assert x % n2 == c2
assert x % n3 == c3

print("Try force(c1, n1)")
force(c1, n1)
print("Try force(c2, n2)")
force(c2, n2)
print("Try force(c3, n3)")
force(c3, n3)
print("Try force(x, n1*n2*n3)")
force(x, n1*n2*n3)
