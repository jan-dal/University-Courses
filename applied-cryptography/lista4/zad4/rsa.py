from gmpy2 import gcd, mpz, powmod, to_binary
from gen import next_prime, ext_euclidean
import secrets
import sys

def generate_rsa_key(k, e=mpz(65537), v=True):
    if v: print("Start generating new rsa keys")
    inv_found = False
    tries = 0
    while(not inv_found):
        tries += 1
        kp = (k+1) // 2

        p_base = (1 << (kp-1)) | secrets.randbits(kp) 
        q_base = (1 << (kp-1)) | secrets.randbits(kp) 
        bits_pg = len(bin(p_base))-2
        bits_qg = len(bin(q_base))-2
        assert bits_pg == kp, f"Number of bits must be: {kp}, but was {bits_pq}" 
        assert bits_qg == kp, f"Number of bits must be: {kp}, but was {bits_qg}"

        p = next_prime(p_base)
        q = next_prime(q_base)
        n = p * q

        bitsn = len(bin(n)) - 2
        assert k-1 <= bitsn 

        michael = (n-p-q+1) // gcd(p-1, q-1)
        r, x, _ = ext_euclidean(e, michael)
        if r != 1:
            continue
        else:
            inv_found = True

        e1 = x % michael
        assert e1*e % michael == 1

    d = e1 % michael
    rbits = 2*tries*kp
    if v: print(f"Done\nIt took {tries} loops and {rbits} random bits")
    return p,q,n,e,d,tries,rbits

def encrypt(m, e, n):
    i = int.from_bytes(m.encode())
    m = mpz(i)
    return powmod(m, e, n)
    
def decrypt(c, d, n):
    m = powmod(c, d, n)
    return bytes.fromhex(hex(m)[2:]).decode()

if __name__ == '__main__':
    k = int(sys.argv[1])
    assert k % 2 == 1, "k must be odd"

    key = generate_rsa_key(k, e=mpz(3))
    print()
    print(f"p: {key[0]}")
    print(f"q: {key[1]}")
    print(f"n: {key[2]}")
    print(f"e: {key[3]}")
    print(f"d: {key[4]}")

    enc = encrypt("Moja wiadomość", key[3], key[2])
    dec = decrypt(enc, key[4], key[2])
    
    print(dec)
