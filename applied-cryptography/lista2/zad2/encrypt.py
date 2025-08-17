import pyaes
import sys

key = "Super secret key".encode("ascii") 
plaintext = sys.argv[1].encode("ascii")
padlen = 32 - len(plaintext) 

print(f"Message {plaintext} has length: {len(plaintext)} -> padding with {hex(padlen)}")
print(f"Second block is: {plaintext[16:32]}")

plaintext += padlen*int.to_bytes(padlen)

print(plaintext.hex())

cbc = pyaes.AESModeOfOperationCBC(key=key)

cipher  = cbc.encrypt(plaintext[:16])
cipher += cbc.encrypt(plaintext[16:])

with open("cipher", "w") as f:
    f.write(cipher.hex())
