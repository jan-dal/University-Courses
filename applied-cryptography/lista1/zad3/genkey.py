#!/bin/python3
import sys
import alph

msg = sys.argv[2]
cipher = sys.argv[1]
key = ""
for i in range(len(msg)):
    for j in alph.abc:
        n = (alph.chr2num[j] + alph.chr2num[msg[i]]) % alph.abcsize
        if n == alph.chr2num[cipher[i]]:
            key += j
            break

print(key)
