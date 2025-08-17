#!/bin/python3

import sys

c1 = sys.argv[1]
c2 = sys.argv[2]

abc = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"

num2chr = dict(enumerate(abc))
chr2num = { num2chr[n]:n for n in num2chr }

m = map(lambda x: num2chr[(chr2num[x[1]] - chr2num[x[0]]) % len(abc)] ,zip(c1, c2[:len(c1)]))

print("".join(list(m)))
