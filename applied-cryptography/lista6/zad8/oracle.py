from gmpy2 import mpz, powmod

class Oracle:
    def __init__(self, key, b):
        self.key = key
        self.d = mpz(key.d)
        self.n = mpz(key.n)
        self.b = b
        self.call = 0

    def decrypt(self, c):
        return powmod(c, self.d, self.n)

    def tell(self, c):
        self.call += 1
        m = self.decrypt(c)
        return m <= self.b
    
    def calls(self):
        return call
