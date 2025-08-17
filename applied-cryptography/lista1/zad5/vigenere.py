#!/bin/python3
import sys
import textwrap

SPECIAL_CHR = "."

cipher = sys.argv[1]
abc = sys.argv[2]
pre = sys.argv[3]

num2chr = dict(enumerate(abc))
chr2num = { num2chr[n]:n for n in num2chr }
abclen = len(abc)


def calc_key(c, t):
    return (chr2num[c] - chr2num[t]) % abclen

def keypart(known_part, offset):
    part = ""
    period = len(known_part)
    for i in range(period):
        if known_part[i] == SPECIAL_CHR:
            part += SPECIAL_CHR
        else:
            part += num2chr[calc_key(cipher[i + offset], known_part[i])]
    t = period - (offset % period)
    part = part[t:] + part[:t] 
    return part


def decode(key):
    keylen = len(key)   
    txt = ""
    for j in range(len(cipher)):
        if key[j % keylen] == SPECIAL_CHR:
            txt += SPECIAL_CHR
        else:
            txt += num2chr[calc_key(cipher[j], key[j % keylen]) % abclen]

    print(txt, key)
    print()
        
def unique(txt):
    txt = txt.replace(".", "")
    return len(set(txt)) == len(txt)

def _color(txt, x, cls):
    if txt.count(x) > 1:
        return f"\033[{str(cls[x])}m{x}\033[00m"
    return x

def color(txt):
    cls = {a[1] : 31 + a[0] for a in enumerate(set(txt))}
    return "".join(map(lambda x: _color(txt, x, cls), txt))

def has_period(txt):
    maxlen = len(txt)
    result = False
    for plen in range(1, maxlen):
        h = True
        for i in range(maxlen - plen):
            if txt[i] != txt[i + plen]:
                h = False
                break
        if h and unique(txt[maxlen-plen:plen]):
            result = True
            break
    return result


def get_key_parts(p):
    keys = []
    for i in range(len(cipher) - len(p) + 1):
        part = keypart(p, i)
        keys.append(part)
        print(color(part), i)
    return keys


for key in get_key_parts(pre):
    decode(key)
decode("PAMIĘTNIK")
