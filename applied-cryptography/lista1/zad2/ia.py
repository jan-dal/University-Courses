import sys
from collections import defaultdict as dd

m1 = sys.argv[1]
m2 = sys.argv[2]

prefixes = dd(lambda: set())
overset  = set()
all_sets = []

def add_to_dict(word):
    for i in range(len(word)):
        prefixes[word[:i]].add(word)

for i in range(1, 21):
    wset = set()
    for word in open(f"dict/{i}"):
        word = word.rstrip()
        wset.add(word)
        overset.add(word)
        add_to_dict(word)

    all_sets.append(wset)

print("DONE")
print(len(prefixes.keys()))
abc = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
abclen = len(abc)

num2chr = dict(enumerate(abc))
chr2num = { num2chr[n]:n for n in num2chr }

def drag(txt, gss):
    guesslen = len(gss)
    a = [chr2num[c] for c in txt]
    g = [chr2num[c] for c in gss]

    cut = a[:guesslen]
    r   = [(a[j] + g[j]) % abclen for j in range(guesslen)]
    result = "".join([num2chr[n] for n in r])
    return result, gss

def get_split(word, wlen):
    r = []
    for i in range(wlen, 0, -1):
        r.append(word[:i])
    return r

def find_word(txt):
    j = len(txt)
    for i in range(1, j):
        w = txt[:j-i]
        if w in overset:
            return w, j-i
    return "", 0

def check_prefix(txt):
    tmp = txt
    off = 0
    words = []
    while not bool(prefixes[tmp]):
        word, l = find_word(tmp)
        if word == "":
            return [], ()
        off += l
        words.append(word)
        tmp = txt[off:]
    return words, tmp

acc = [[] for i in range(20)]

def do(x):
    for wordlen in range(20, 6, -1):
        for word in all_sets[wordlen-1]:
            result = drag(x, word)
            r = result[0]
            spl = get_split(r, wordlen)
            for s in spl:
                if s in overset:
                    l = len(s)
                    w = check_prefix(r[l:])
                    if bool(w[1]):
                        q = s, w[0], w[1], result[1]
                        acc[wordlen-1].append(q)
                        break

do(m2)
do(m1)

for w in acc:
    for l in w:
        print(l)
