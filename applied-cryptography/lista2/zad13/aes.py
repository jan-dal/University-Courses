from util import *
import pyaes
import sys

fname  = sys.argv[1]
mode = sys.argv[2]
e = sys.argv[3]

key = "Some aes key1234".encode()

with open(fname, "rb") as f:
    img = f.read()

aes = None

if mode == "ctr":
    aes = pyaes.AESModeOfOperationCTR(key)
elif mode == "ecb":
    aes = pyaes.AESModeOfOperationECB(key)
else:
    aes = pyaes.AESModeOfOperationCBC(key)


if e == "encrypt":
    cipher = encrypt(img, aes)
    name = f"{fname}.{mode}"
else:
    cipher = decrypt(img, aes)
    name = f"{fname}.decrypted"

with open(name, "wb") as w:
    w.write(cipher)
