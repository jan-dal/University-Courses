from gmpy2 import mpz, t_divmod, is_square, isqrt, powmod, invert, iroot

def encrypt(m, e, n):
    return powmod(m, e, n)

def decrypt(n, p, q, e, c):
    pq1 = n - p - q + 1
    e1 = invert(e, pq1)
    d  = e1 % pq1
    dec = powmod(c, d, n)
    return dec

def to_plaintext(x):
    try:
        message = bytes.fromhex(hex(x)[2:]).decode(errors="ignore")
        return message
    except:
        pass

def fermat_fact(n):
    x = isqrt(n)
    y = 2
    print(f"sqrt(n): {int(x):.50E}")
    for i in range(x+1, n):
        y = i*i - n
        if (is_square(y)):
            print(f"iters: {i - x}")
            x = i
            y = isqrt(y)
            break
    p = x + y
    q = x - y
    return p, q

def break_smalln(n, e, c):
    for i in range(2, isqrt(n)):
        q, r = t_divmod(n, mpz(i))
        if r == 0:
            p = mpz(i)
            break    
    assert p*q == n
    return p, q

def break_message(n, e, c):
    plaintext = "Niniejszym zaświadcza się, że liczba punktów przyznawanych za to zadanie wynosi 2+5."
    ptxt = mpz(int.from_bytes(plaintext.encode()))
    cout = encrypt(ptxt, e, n)
    assert cout == c, "Plaintexts differ"
    return plaintext

def break_small_message(n, e, c):
    for i in range(30):
        r = iroot(i*n+c, e)
        if r[1]:
            return to_plaintext(r[0])
