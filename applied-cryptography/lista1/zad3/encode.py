#!/bin/python3

import sys
from random import choices

abc = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"

if len(sys.argv) > 2:
    key = sys.argv[2]
else:
    key  = "".join(choices(abc, k=len(sys.argv[1])))

num2chr = dict(enumerate(abc))
chr2num = { num2chr[n]:n for n in num2chr }
def encodeChar(c, k):
    return num2chr[(chr2num[c]+chr2num[k]) % len(abc)]

def encode(plaintext, key):
    ciphertext = [encodeChar(plaintext[n], key[n]) for n in range(len(plaintext))]
    return "".join(ciphertext)

print(encode(sys.argv[1], key))
