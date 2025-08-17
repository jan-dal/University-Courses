from gmpy2 import powmod, mpz, mpq, invert
from Crypto.PublicKey import RSA

class Server:

    def __init__(self, bits, e):
        key = RSA.generate(bits, e=e)
        self.n = mpz(key.n)
        self.d = mpz(key.d)
        self.e = mpz(e)

    def half(self, c):
        m = powmod(c, self.d, self.n)  
        if ((m << 1) < self.n):
            return 0
        return 1

    def parity(self, c):
        m = powmod(c, self.d, self.n)
        return m % 2

    def encrypt(self, message):
        m = mpz(int.from_bytes(message.encode()))
        return powmod(m, self.e, self.n)
    
    def get_pub_key(self):
        return self.e, self.n

server = Server(1024, 65537)
message = "Some message that needs to be decrypted by the half oracle method. pad pad pad pad pad pad"
c = server.encrypt(message)
e, n = server.get_pub_key()

def attack_oracle():
    r = [mpq(0), mpq(1)]
    i2 = powmod(invert(2, n), e, n)
    for s in range(int(n).bit_length()):
        h = c*powmod(mpz(1 << s), e, n) % n
        p = h*powmod(2, e, n) % n
#        x = server.half(h)
#        x = server.parity(p)
        k = r[0] + r[1]
        if x:
            r[0] = k / 2
        else:
            r[1] = k / 2
    return n * r[1]

m = attack_oracle()
m = bytes.fromhex(hex(mpz(m))[2:])
print(m.decode())
