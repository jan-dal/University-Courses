from RM import *
import sys

M = int(sys.argv[1])
N = int(sys.argv[2])

def print_stat(x, n, sort_by):
    sort = ["\u2193" if i == sort_by else "" for i in range(3)]
    print(f"Number{sort[0]} | absolute count{sort[1]} | relative count{sort[2]}")
    for i in range(min(n, len(x))):
        print(f"{x[i][2]}\t{x[i][0]}\t{x[i][1]*100:.2f}%")

truths = list(map(lambda x: not is_prime(x), range(M, N+1)))
liars = [rabin_miller_liars(i, truths[i-M]) for i in range(M, N+1)] 

liars_abs = sorted(liars, reverse=True)
liars_rel = sorted(liars, reverse=True, key=lambda x: x[1])
print_stat(liars_abs, 10, 1)
print_stat(liars_rel, 10, 2)

print(f"Average absolute count: {sum([x[0] for x in liars_abs])/len(liars_abs):.2f}")
print(f"Average relative count: {sum([x[1] for x in liars_rel])/len(liars_rel)*100:.2f}%")
