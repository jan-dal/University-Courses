from random import randint, choice
import os

def randomtest():
    s = ""
    ch = ['.', 'x']
    L = []
    n = randint(4,4)

    L += [str(n)+"\n"]

    for i in range(n):
        for k in range(3):
            s += choice(ch)
        s += "\n"
        L += s
        s = ""

    f = open("gentest", "w+")

    for i in L:
        f.write(i)
    f.close()

for i in range(1):
    randomtest()
    os.system('./a.out < gentest')