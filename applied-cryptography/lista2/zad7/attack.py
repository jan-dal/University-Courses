import pyaes
import sys
from util import * 

message = "==Shopping list==1. 500g celery  2. 30 eggs 3. 1kg of flour 4. 12 apples 5. Fish".encode()
p = ["1. 201 bananas  ".encode(), "3kg of almonds  ".encode()]

key = "Some random key ".encode()
cbc_encrypt = pyaes.AESModeOfOperationCBC(key)
cbc_decrypt = pyaes.AESModeOfOperationCBC(key)
def xor(a, b, c):
    return bytes(a ^ b ^ c for a, b, c in zip(a, b, c))

def eve(message, cipher, p):    
    new_cipher = bytearray(cipher)
    for i in range(len(p)):
        part = cipher[32*i:32*i+16]
        m   = message[32*i+16:32*(i+1)]
        new_cipher[32*i:32*i+16] = xor(part, m, p[i])
    
    return bytes(new_cipher)

cipher = encrypt(message, cbc_encrypt)
manipulated = eve(message, cipher, p)
print(decrypt(manipulated, cbc_decrypt).decode(errors="ignore"))
