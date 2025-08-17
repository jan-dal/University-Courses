
f = open("maxtest2","w")
f.write("1\n6377552\n")

def Rtree(n, v, s):
	for i in range(s, s+n):
		f.write(f"{v} {i}\n")



for k in range(1):
	s = 2
	for i in range(1, 127552):
		

		Rtree(50, i, s)
		s += 50


f.write("1 6377552\n")


for k in range(1):
	s = 2
	for i in range(1, 127552):
		

		Rtree(50, i, s)
		s += 50


f.write("6377551 6377552\n")

f.close()
