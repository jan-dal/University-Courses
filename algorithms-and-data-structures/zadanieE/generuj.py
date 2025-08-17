#!/bin/python

from random import randint

f = open("testrandom2", "w")

n = 10000

f.write(f"{n}\n")

for i in range(n):
	x = randint(-10**7, 10**7)
	y = randint(-10**7, 10**7)
	f.write(f"{x} {y}\n")

f.close()
