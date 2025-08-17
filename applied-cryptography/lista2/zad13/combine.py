import sys

fname1 = sys.argv[1]
fname2 = sys.argv[2]

def xor(x, y):
    return bytes(a ^ b for a, b in zip(x, y))

with open(fname1, "rb") as f:
    cipher1 = f.read()
with open(fname2, "rb") as f:
    cipher2 = f.read()

r = xor(cipher1, cipher2)

with open("combined.data", "wb") as w:
    w.write(r)
