#!/bin/python3

import sys
from random import choices

abc = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
encode = True if sys.argv[3] == "encode" else False 
key = sys.argv[2]
txt = sys.argv[1]

if (len(key) == 0):
    key  = "".join(choices(abc, k=len(txt)))

num2chr = dict(enumerate(abc))
chr2num = { num2chr[n]:n for n in num2chr }

def encodeChar(c, k):
    return num2chr[(chr2num[c]+chr2num[k]) % len(abc)]

def decodeChar(c, k):
    return num2chr[(chr2num[c]-chr2num[k]) % len(abc)]

def cipher(txt, key, encode=True):
    if (encode):
        result = [encodeChar(txt[n], key[n]) for n in range(len(txt))]
    else:
        result = [decodeChar(txt[n], key[n]) for n in range(len(txt))]
    return "".join(result)

print(cipher(txt, key, encode))
