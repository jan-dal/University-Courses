import sys
from util import decrypt 
# p' xor p xor c => c'

cfile = sys.argv[1]
pfile  = sys.argv[2]
sfile  = sys.argv[3]

y1 = int(sys.argv[4])
y2 = int(sys.argv[5])

def xor(x, y, z):
    return bytearray(a ^ b ^ c for a, b, c in zip(x, y, z))

with open(cfile, "rb") as f:
    cipher = f.read()
with open(pfile, "rb") as f:
    plain = f.read()
with open(sfile, "rb") as f:
    tamper = f.read()

new_cipher = xor(tamper, plain, cipher)

new_cipher[:1024*y1] = cipher[:1024*y1]
new_cipher[1024*y2:] = cipher[1024*y2:]

with open(f"{pfile}.tampered", "wb") as w:
    w.write(new_cipher)
