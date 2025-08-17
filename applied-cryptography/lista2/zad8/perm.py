import random
import sys
import matplotlib.pyplot as plt
from collections import defaultdict as dd
from math import log2
n = int(sys.argv[1])

def generate_num(maxnum):
    k = int(log2(maxnum)) + 1
    bits = random.getrandbits(k)
    while(bits > maxnum):
        bits = random.getrandbits(k)

    return bits


def generate_perm(n):
    nums = []
    while(len(nums) < n):
        num = generate_num(n-1)
        if num not in nums:
            nums.append(num)

    return nums


def test(n, tries=100000):
    d = dd(lambda: 0)
    for i in range(tries):
        result = tuple(generate_perm(n))
        d[result] += 1  
        print(f"{i}/{tries}", end="\r")
    print()
    return d

def plot_result(r):
    y = r.values()
    plt.bar(range(len(r)), y) 
    plt.show()

result = test(n)
plot_result(result)

