from gmpy2 import mpz, powmod, next_prime
import os


p = next_prime(mpz(int.from_bytes(os.urandom(512))))
q = next_prime(mpz(int.from_bytes(os.urandom(512))))
e = mpz(3)





