from gmpy2 import floor, mpz, mpq, mpfr

class ContFrac:

    def __init__(self, x, max_iter=100):
        self.x = x
        self.max_iter = max_iter

    def __iter__(self):
        if isinstance(self.x, float):
            self.xi = mpfr(self.x)
        else:
            self.xi = mpq(self.x, 1)

        self.i = 0
        self.ai = mpz(self.x)
        self.p2, self.q2, self.p1, self.q1 = 0, 1, 1, 0
        self.pi, self.qi = mpz(self.x), mpz(1)

        return self

    def __next__(self):
        if self.i >= self.max_iter:
            raise StopIteration
        
        self.ai = mpz(floor(self.xi))
        self.pi = self.ai * self.p1 + self.p2
        self.qi = self.ai * self.q1 + self.q2
        self.p1, self.p2 = self.pi, self.p1
        self.q1, self.q2 = self.qi, self.q1
        self.i += 1
       
        try:
            self.xi = 1 / (self.xi - self.ai)
        except ZeroDivisionError:
            print("Division by zero reached!")
            self.i = self.max_iter

        return self.pi, self.qi

