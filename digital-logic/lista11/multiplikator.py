

def add(L,a,b):
	
	jedynki = ("".join(L) + a + b).strip("0")
	
	if len(jedynki) % 2 == 0:
		result = "1"
	else:
		result = "0"
		
	L = ["1" for i in range(len(jedynki) // 2)]
	
	return L, result


def mul(x,y):
	if x == "1" and y == "1":
		return 1
	else: 
		return "0"
	
def multiply(a,b):

	a = bin(a)
	b = bin(b)
	
	if len(a) < len(b):
		a, b = b, a
	
	a = [x for x in a if x != 'b']
	a = a[1:]
	b = [x for x in b if x != 'b']
	b = b[1:]
	
	if len(a) < 16:
		a = ["0" for i in range(16-len(a))] + a
	if len(b) < 16:
		b = ["0" for i in range(16-len(b))] + b
	
	print(a,b)
	wyniki = []
	for i in range(-1,-len(b),-1):
		wynik = []
		for k in range(-1,-len(a),-1):
			wynik = [mul(a[k],b[i])] + wynik
		wyniki.append(wynik)
	
	return wyniki
	
print(multiply(4,5))
