#!/bin/python3
import sys
import subprocess
from math import log2

cipher = sys.argv[1]
whole_drag = sys.argv[2]
guess = None
offset = 0

if whole_drag == "-d":
    guess = sys.argv[3]
    whole_drag = True
else:
    whole_drag = False
    if len(sys.argv) > 2:
        offset = int(sys.argv[2])
    if len(sys.argv) > 3:
        p = subprocess.run("grep -E " + sys.argv[3] + " /usr/share/dict/pl", encoding="UTF-8", shell=True, capture_output=True)
        guess = p.stdout
        guess = list(map(str.upper, guess.split("\n")))[:-1]
        if len(guess) == 0:
            guess.append("")

if len(sys.argv) > 4:
    guess = list(map(lambda x: sys.argv[4] + x, guess))

abc = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
abclen = len(abc)

num2chr = dict(enumerate(abc))
chr2num = { num2chr[n]:n for n in num2chr }


def load_dict(path):
    wdict = set()
    for word in open(path):
        word = word.strip().upper()
        add = True
        for l in word:
            if l not in abc:
                add = False
                break
        if add:
            wdict.add((word, len(word)))
    return wdict

def drag(txt, gss, offset):
    guesslen = len(gss)
    a = [chr2num[c] for c in txt]
    g = [chr2num[c] for c in gss]

    cut = a[offset:offset+guesslen]
    r   = [(a[offset+j] + g[j]) % abclen for j in range(guesslen)]  
        
    result = "".join([num2chr[n] for n in r])
    return result, gss, offset

def has_word(x, minlen):
    for t in common_words:
        word = t[0]
        wordlen = t[1]
        cut = x[:wordlen]
        if wordlen >= minlen and word == cut:
            return True
    return False

def crib_drag(txt, gss):
    acc = []
    for i in range(len(txt) - len(gss) + 1):
        acc.append(drag(txt, gss, i))
    return acc

def force(off):
    acc = []
    for word in common_words:
        if word[1] > 2:
             acc.append(drag(cipher, word[0], off))

    result = []
    acclen = len(acc)
    i = 0
    for t in acc:
        i += 1
        result.append(str(t))
        print(f"{i}/{acclen}", end="\r")
    print()
    print(f"Found: {len(result)} offset: {off}")
    print("\n".join(result))

if guess == None:
    common_words = load_dict("common")
    for i in range(1):
        force(i)
elif whole_drag:
    r = crib_drag(cipher, guess)
    print("\n".join(list(map(str, r))))
else:
    for g in guess:
       print(drag(cipher, g, offset))
