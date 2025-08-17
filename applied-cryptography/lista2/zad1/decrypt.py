import pyaes
import sys

cpath = sys.argv[1]
iv  = bytes.fromhex(sys.argv[2])
key = bytes.fromhex(sys.argv[3])


cbc = pyaes.AESModeOfOperationCBC(key, iv=iv)

# Można zaszyfrować 16 bajtów za każdym razem 
# plaintext = aes.decrypt(cipher)

# Czytanie i pisanie plików
# pyaes.decrypt_stream(cbc, filein, fileout)

decrypter = pyaes.Decrypter(cbc)

plaintext = bytes()
for l in open(cpath):
    plaintext += decrypter.feed(bytes.fromhex(l))

# Usuń nadmiar bajtów i dodaj wyrównanie
decrypter.feed()

print(plaintext.decode())
