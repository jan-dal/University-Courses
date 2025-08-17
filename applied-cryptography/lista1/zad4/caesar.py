#!/bin/python3
import sys

def encode_ces(cipher):
    abc = "".join(list(map(chr, range(65, 91))))
    shft = abc
    res = []

    for i in range(len(abc)):
        shft = shft[-1] + shft[:len(shft)-1]
        tr = str.maketrans(abc, shft)
        res.append((cipher.translate(tr), shft))
    return res

if __name__ == "__main__":
    print("\n".join(["\t".join(r) for r in encode_ces(sys.argv[1])]))
