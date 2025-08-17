#!/bin/python3
import sys

txt = sys.argv[3]
s   = int(sys.argv[1])
mx  = int(sys.argv[2])

done = set()

for i in range(len(txt) - s + 1):
    j = 0
    while(mx >= j and i+j < len(txt)):
        j += 1
    j -= 1
    if j >= s:
        cut = txt[i:i+j]
        skip = False
        for word in done:
            if cut in word:
                skip = True

        if not skip:
            done.add(cut)
            occ = txt.count(cut)
            print(occ, cut)
             
