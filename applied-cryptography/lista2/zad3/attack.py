from util import *
import pyaes
import os

plaintext = "Send $100 to Bob from my account.               ".encode("ascii")
key = "AComplicatedKey ".encode("ascii")
iv  = os.urandom(16)

cbc = pyaes.AESModeOfOperationCBC(key, iv)
cipher  = encrypt(plaintext, cbc)

def send_to_bank(cipher, iv):
    cbc = pyaes.AESModeOfOperationCBC(key, iv)
    message  = decrypt(cipher, cbc)

    print(message.decode("ascii"))


def eve(cipher, iv):
    # Decrypting the cipher:
    # c1 -> decrypt -> xor with iv -> plaintext
    x = bytes(a ^ b for a, b in zip(iv[13:], "Bob".encode("ascii")))
    change = bytes(a ^ b for a, b in zip(x, "Eve".encode("ascii")))
    new_iv = iv[:13] + change

    send_to_bank(cipher, new_iv)

eve(cipher, iv)
