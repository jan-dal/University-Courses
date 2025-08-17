#!/bin/python3
import sys

key = sys.argv[1]
txt_part = sys.argv[2]
cipher = sys.argv[3]

tr_tab = str.maketrans(key, txt_part) 

def color(x):
    if x in key:
        return "\033[91m" + x + "\033[00m"
    return x

print("".join(list(map(color, cipher))).translate(tr_tab))
