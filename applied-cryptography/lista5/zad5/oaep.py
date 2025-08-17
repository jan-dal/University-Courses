import Crypto.PublicKey.RSA as RSA
import hashlib
import os
import secrets
from math import ceil
from gmpy2 import mpz, powmod

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

n = mpz(132976518568428873590990698990473888147434186684055790647542257711183504300793567894763258123494558264011608829984042495270894713938736746289613125052979025936004467648488790223063947335152106970185641201512558554026954048393654332511691849974991216693754764137851682541757604893914324078124535978192415120291)
e = mpz(65537)
d = mpz(90691539900408400794917546924290269291024396023883328113480877869245908291996431087663966131282119577132411918058451674796270519384770684482152644315318102571397196010481370395695226323681136902628494694117140916057608574759453391255173778818496321940136314911468520140003952162724505217290328347539832915473)
c = 32285987280270223193394300987555362945882719223146559544577770972899088517077123906704034961613752708098754728140271941658235688552502353529258875148446114782922280442774871977472579771277610967830632811877354856461374593809829697266148408396441603567328790550104677162545537524254825671420002401958272746803
c = i2osp(c, (len(n) + 7) // 8)

print(f"Ciphertext: {c}.")    
print("Decrypting.")
dm = rsa_oaep_decrypt(c, d, n)
print(f"Decrypted message: \"{dm.decode()}\".")
