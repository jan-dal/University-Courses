abc = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
abcsize= len(abc)
num2chr = dict(enumerate(abc))
chr2num = { num2chr[n]:n for n in num2chr }
