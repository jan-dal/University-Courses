#!/bin/python3

import sys
import textwrap
cipher = sys.argv[1]
abc = sys.argv[2]

def coincidence(crpt, m):
    parts = textwrap.wrap(crpt, m)
    
    if len(parts[-1]) != len(parts[0]):
        parts = parts[:-1]
    
    N = len(parts)
    
    ind = []
    for i in range(m):
        dc = {l : 0 for l in abc}
        for part in parts:
            dc[part[i]] += 1
        
        ind.append(len(abc) * sum([dc[n] * (dc[n] - 1) for n in abc ]) / (N * (N-1)))

    return sum(ind) / len(ind)
    
for i in range(1, 30):
    print(coincidence(cipher, i))
