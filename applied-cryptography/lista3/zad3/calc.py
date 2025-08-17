from decimal import *

getcontext().prec = 1024

def stirling(n, k):
    acc = Decimal(0)
    kj  = Decimal(1)

    for i in range(k+1):
        if (i % 2 == 0):
            acc += kj*Decimal(k - i)**n
        else:
            acc -= kj*Decimal(k - i)**n
        kj *= Decimal(k - i)/Decimal(i+1)
    return acc / factorial(k)

def part_fact(a, b):
    if a == b:
        return a
    if a+1 < b:
        m = (a + b) // 2
        return part_fact(a, m) * part_fact(m+1, b)
    return a*b

def factorial(n):
    if n < 2:
        return Decimal(1)
    return part_fact(Decimal(1), Decimal(n))

def b(x, r):
    t = Decimal(x)
    result = Decimal(0)
    for k in range(1, r+1):
        a = Decimal(x - k)**r
        result += a*t*stirling(r, k)
        t *= Decimal(x - k)
    return result

def calculate(x, r):
    return b(x, r) / Decimal(x)**(2*r)
print(calculate(2**32, 4))
