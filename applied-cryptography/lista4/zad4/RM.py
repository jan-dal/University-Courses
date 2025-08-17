from gmpy2 import mpz, is_prime, c_mod, t_mod, powmod
import secrets

def _rabin_miller(p, a):
    s = 0
    d = p - 1
    while(d % 2 == 0):
        d >>= 1
        s += 1
    if(s == 0):
        return True

    pmpz = mpz(p)
    ad = powmod(mpz(a), mpz(d), pmpz)
    x = t_mod(ad, pmpz)

    if(x == 1):
        return False
     
    for i in range(s):
        x = c_mod(ad, pmpz)
        if (x == -1):
            return False
        ad *= ad

    return True

def rabin_miller(p, repeat=1):
    if p < 4:
        return False
    for _ in range(repeat):
        a = secrets.randbelow(p-3) + 2
        
        result = _rabin_miller(p, a)
        if result == False:
            break
    
    return result

def rabin_miller_liars(p, truth):
    if p < 4:
        return 0, 0, p
    c = 0
    for a in range(2, p-1):
        result = _rabin_miller(p, a)
        if (result != truth): c += 1
    
    return c, c / p, p

