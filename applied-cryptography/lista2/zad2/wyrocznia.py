import pyaes
import sys

key = "Super secret key".encode("ascii")
cbc = pyaes.AESModeOfOperationCBC(key)

def tell(cipher):
    plaintext  = cbc.decrypt(cipher[:16])
    plaintext += cbc.decrypt(cipher[16:])
    
    lastbyte = plaintext[31]
    
    if (int(lastbyte) == 0):
        return False

    for i in range(int(lastbyte)):
        if plaintext[31 - i] != lastbyte:
            return False
    print(plaintext.hex())
    return True

if __name__ == "__main__":
    tell(sys.argv[1])
