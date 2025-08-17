def wypisz(wynik):
	for i in wynik:
		print("    ",end="")
		for k in i[2]:
			print(k," ",end="")
		print()
		print(i[1][0],end="  ")
		for u in range(4):
			print(i[0][u]+"   ",end="")
		print()
		print(i[1][1],end="  ")
		for u in range(4,8):
			print(i[0][u]+"   ",end="")
		print()
		print(i[1][2],end="  ")
		for u in range(8,12):
			print(i[0][u]+"   ",end="")
		print()
		print(i[1][3],end="  ")
		for u in range(12,16):
			print(i[0][u]+"   ",end="")
		print()
		print()
		print()

def binar(n):
	c = []
	for i in range(2**n):
		h = ""
		for k in range(n):
			h += (str(i%2) + " ")
			i //= 2
		c.append(h[::-1])
	return c

def oblicz(a):
	s = 0
	for i in range(len(a)):
		if a[i] == "1":
			s += 2**(len(a)-1-i)
	return s
	
def typ4():
	ciagi = [("00","01","10","11"),
			 ("00","10","01","11"),
			 ("01","11","10","00"),
			 ("01","00","10","11"),
			 ("10","11","01","00"),
			 ("10","00","01","11"),
			 ("11","01","10","00"),
			 ("11","10","01","00"),
			 ("00","01","11","10"),
			 ("00","10","11","01"),
			 ("11","01","00","10"),
			 ("11","10","00","01"),]
	c = binar(4)
	wyniki = []
	uzyte = set()
	wart = []
	print(" x y z w")
	for i in range(16):
		wart.append((input(c[i] + " ")))
	for k in ciagi:
		for i in ciagi:
			x = []
			for e in k:
				for u in i:
					x.append(wart[oblicz(e+u)])
		if not "".join(x) in uzyte: 
			wyniki.append([x,k,i])
			uzyte.add("".join(x))
		
	wypisz(wyniki)
	
	
if __name__ == "__main__":
	a = input("Wybierz rozmiar(2,3,4): ")
	
	if a == "4":
		typ4()
