import random
from math import log2

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
