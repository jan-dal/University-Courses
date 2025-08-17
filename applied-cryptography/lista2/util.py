import pyaes

def encrypt(message, aes):
    encrypter = pyaes.Encrypter(aes)
    cipher  = encrypter.feed(message)
    cipher += encrypter.feed()
    return cipher

def decrypt(cipher, aes):
    decrypter = pyaes.Decrypter(aes)
    message  = decrypter.feed(cipher)
    message += decrypter.feed()
    return message
