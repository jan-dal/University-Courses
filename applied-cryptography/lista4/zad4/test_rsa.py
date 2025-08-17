from rsa import *
import sys

bit = int(sys.argv[1])
e   = mpz(sys.argv[2])

def test(N=100):
    acc = 0
    for i in range(N):
        result = generate_rsa_key(bit, e=e, v=False)
        acc += result[-1]

    return acc / N

def chance(t):
    return 1 - (1/e)**t


for i in range(1, 6):
    c = chance(i)
    print(f"{i*(bit+1)} bits\t{c*100:.2f}%")

print(f"It took an average of {test()} random bits")
