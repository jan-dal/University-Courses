from hashlib import sha256
from itertools import product
from time import time 

with open("zad1a.html", "rb") as f:
    html1 = f.read()
with open("zad1b.html", "rb") as f:
    html2 = f.read()

prefix = sha256(html1).hexdigest()[:8]
c = bytearray('a'.encode()) * 8

def check():
    global html2, prefix
    html2 = html2[:43] + c + html2[51:110] + c + html2[118:]
    d = sha256(html2).hexdigest()[:8]
    if prefix == d:
        print(f"Found 8 {c}!")
        w = open("8.html", "wb")
        w.write(html2)
        w.flush()
        w.close()
    
    elif prefix[:7] == d[:7]:
        print(f"Found 7 {c}!")
        w = open("7.html", "wb")
        w.write(html2)
        w.flush()
        w.close()

        
def loop(idx):
    if (idx <= -1): return
    for i in range(97, 114):
        c[idx] = i
        check()
        loop(idx-1)
loop(7)
w.close()

