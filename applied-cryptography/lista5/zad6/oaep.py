import Crypto.PublicKey.RSA as RSA
import hashlib
import os
import secrets
from math import ceil
from gmpy2 import mpz, powmod
from cfrac_attack import *

SHA256_LEN = 256
h = SHA256_LEN // 8
hL = bytes.fromhex("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855")

def xor(a, b):
    return bytes(x ^ y for x, y in zip(a, b))

def i2osp(x, xlen):
    assert x < 256**xlen, "integer too large"
    return x.to_bytes(xlen, 'big')

def os2ip(x):
    return int.from_bytes(x, 'big')

def rsadp(c, d, n):
    assert 0 <= c < n, "ciphertext representative out of range" 
    return powmod(c, d, n)

def rsaep(m, e, n):
    assert 0 <= m < n, "message representative out of range"
    return powmod(m, e, n)

def mgf(mgfSeed, maskLen):
    assert maskLen <= (1 << 32), "mask too long"
    T = bytes()
    for c in range(ceil(maskLen/h)):
        d = mgfSeed + i2osp(c, 4)
        T += hashlib.sha256(d).digest()
    return T[:maskLen]

def rsa_oaep_encode(m, n):
    k  = (len(n)+7) // 8
    mLen = len(m)
    max_len = k - 2*h - 2
    assert mLen <= max_len, f"message too long"
    
    DB = hL + bytes(k-mLen-2*h-2) + b'\x01' + m

    seed = 1 << (SHA256_LEN-1) | secrets.randbits(SHA256_LEN)
    seed = seed.to_bytes(h)

    dbMask = mgf(seed, k-h-1) 
    maskedDB = xor(DB, dbMask)

    seedMask = mgf(maskedDB, h)
    maskedSeed = xor(seed, seedMask)

    return b'\x00' + maskedSeed + maskedDB

def rsa_oaep_encrypt(em, e, n):
    k = (len(n)+7) // 8
    m = os2ip(em)
    c = int(rsaep(mpz(m), e, n))
    return i2osp(c, k)


def rsa_oaep_decode(EM, k):
    Y = EM[0]
    maskedSeed = EM[1:1+h]
    maskedDB = EM[1+h:]

    seedMask = mgf(maskedDB, h)
    seed = xor(maskedSeed, seedMask)

    dbMask = mgf(seed, k-h-1)
    DB = xor(maskedDB, dbMask)
        
    lHash = DB[:h]
    assert lHash == hL
    
    m_start = DB[:].find(b'\x01')+1
    return DB[m_start:]


def rsa_oaep_decrypt(c, d, n):
    cl = len(c)
    kl = (len(n) + 7) // 8

    assert kl == cl, "decryption error"
    assert kl >= 2*h+2, "decryption error"
    
    c = os2ip(c)
    m = int(rsadp(c, d, n))
    em = i2osp(m, kl)
    
    return rsa_oaep_decode(em, kl)

def to_plaintext(x):
    try:
        return x.decode(errors="ignore")
    except:
        return "Exception"

n = mpz(71502864762146481719921706717489647274401371264339912861896668788523408898399987584458867427150377982863182827723210496949296670656691865361398662494882040185240162250557186910283675756901011497105524781775877739937033106782971695711279690633676059066099291799230305634094313871898669144926153021806765337119)
e = mpz(2021535990211590963017392592316424966597438880474232111192775168758971118971196922496380667821335075997803106339714897610001405009507387105725190113425251075371250635294214688303728478757021436787448387590713186179971049230570754206241716782351576777192891629109675421410740010665647739359531488881247608893)
c = 60342465832449322744726634842148045802439244500089483738363609568925340232650344873572852511944450706598604355605661206516319914717158123597735277322179464919791400999160339273677529535935467512005475661904854431907925955377288727809206617991218845692977472510923562573590533382301718414219836335384794307181 
c = i2osp(c, (len(n) + 7) // 8)

print(f"Ciphertext: {c}.")    
print("Breaking short d exponent.")
p, q, t = break_rsa(e, n)
d = invert(e, mpz(p-1)*mpz(q-1))
print("Success!")
print(f"p = {p}")
print(f"q = {q}")
print("Decrypting.")
dm = rsa_oaep_decrypt(c, d, n)
print(f"Decrypted message: \"{dm.decode()}\".")
