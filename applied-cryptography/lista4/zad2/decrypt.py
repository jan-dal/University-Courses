from gmpy2 import mpz, powmod, invert
import asn1

with open("key.der", "rb") as f:
    pk = f.read()

decoder = asn1.Decoder()
decoder.start(pk)
decoder.enter()
tag, val = decoder.read()

n = mpz(decoder.read()[1])
e = mpz(decoder.read()[1])
d = mpz(decoder.read()[1])
p = mpz(decoder.read()[1])
q = mpz(decoder.read()[1])
dp = mpz(decoder.read()[1])
dq = mpz(decoder.read()[1])
qinv = mpz(decoder.read()[1])

assert p * q == n

e1 = invert(e, (p-1)*(q-1))
assert d == e1 % ((p-1)*(q-1))

assert dp == e1 % (p-1)

assert dq == e1 % (q-1)

q1 = invert(q, p)
assert qinv == q1 % p

with open("cipher", "r") as f:
    cipher = bytes.fromhex(f.read())
cipher = int.from_bytes(cipher, byteorder="big")

decrypt = bytes.fromhex(hex(powmod(cipher, d, n))[2:])

print(decrypt.decode())
