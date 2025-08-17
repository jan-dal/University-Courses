# 2^8!   => 10^506          log => 1683
# 2^16!  => 10^287193       log => 954036
# 2^32!  => 10^39507966976  log => 10^11
# 2^64!  => 10^10^20        log => 10^21
# 2^128! => 10^10^40        log => 10^40


from perm import *

key = generate_perm(256)
iv  = "q"
message = "Some secret message for your eyes only"
ctr_message = "a"*512

def xor(a, b):
    return "".join([chr(ord(x) ^ ord(y)) for x, y in zip(a, b)])

class ECB:

    def encrypt(self, message, key):
        cipher = ""
        for c in message:
            cipher += chr(key[ord(c)])
        return cipher

    def decrypt(self, cipher, key):
        message = ""
        for c in cipher:
            message += chr(key.index(ord(c)))
        return message

class CBC:

    def __init__(self):
        self.ecb = ECB()

    def encrypt(self, message, key, iv):
        cipher = ""
        for i in range(len(message)):
            p = xor(message[i], iv)
            c = self.ecb.encrypt(p, key)
            cipher += c
            iv = c
        return cipher

    def decrypt(self, cipher, key, iv):
        message = ""
        for i in range(len(cipher)):
            d = self.ecb.decrypt(cipher[i], key)
            p = xor(d, iv)
            iv = cipher[i]
            message += p
        return message

class CFB:
    
    def __init__(self):
        self.ecb = ECB()

    def encrypt(self, message, key, iv):
        cipher = ""
        for i in range(len(message)):
            p = self.ecb.encrypt(iv, key)
            c = xor(message[i], p)
            iv = c
            cipher += c
        return cipher

    def decrypt(self, cipher, key, iv):
        message = ""
        for i in range(len(cipher)):
            d = self.ecb.encrypt(iv, key)
            p = xor(cipher[i], d)
            iv = cipher[i]
            message += p
        return message

class OFB:
    def __init__(self):
        self.ecb = ECB()

    def encrypt(self, message, key, iv):
        p = ""
        for i in range(len(message)):
            d  = self.ecb.encrypt(iv, key)
            iv = d
            p += d
        return xor(message, p)

    def decrypt(self, cipher, key, iv):
        return self.encrypt(cipher, key, iv)

class CTR:

    def __init__(self):
        self.ecb = ECB()

    def encrypt(self, message, key):
        cipher = ""
        iv = 0
        for i in range(len(message)):
            p = ecb.encrypt(chr(iv), key)
            cipher += xor(p, message[i])
            iv += 1
            iv %= 256
        return cipher

    def decrypt(self, cipher, key):
        return self.encrypt(cipher, key)

print("ECB")
ecb = ECB()
ecb_c = ecb.encrypt(message, key)
print(ecb_c)
print(ecb.decrypt(ecb_c, key))
print()

print("CBC")
cbc = CBC()
cbc_c = cbc.encrypt(message, key, iv)
print(cbc_c)
print(cbc.decrypt(cbc_c, key, iv))
print()

print("CFB")
cfb = CFB()
cfb_c = cfb.encrypt(message, key, iv)
print(cfb_c)
print(cfb.decrypt(cfb_c, key, iv))
print()

print("OFB")
ofb = OFB()
ofb_c = ofb.encrypt(message, key, iv)
print(ofb_c)
print(ofb.decrypt(ofb_c, key, iv))
print()

print("CTR")
ctr = CTR()
ctr_c = ctr.encrypt(ctr_message, key)
print(ctr_c)
print(ctr.decrypt(ctr_c, key))
