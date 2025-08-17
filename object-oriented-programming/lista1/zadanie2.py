def skroc(u):
	num, denom = u[0], u[1]
	a,b = abs(num),abs(denom)
	for i in range(2,min(a,b)+1):
		while a % i == 0 and b % i == 0:
			a /= i
			b /= i
	a, b = int(a),int(b)
	if num < 0 and denom > 0:
		return [-a,b]
	elif num > 0 and denom < 0:
		return [a,-b]
	else:
		return [a,b]

def czy_liczba(*args):
	for i,k in args:
		if not (type(i) == type(1) or type(k) == type(1)):
			print(f"{i} nie jest poprawnym typem.")
			return False
	return True

def nowy_ulamek(num,denom):	
	assert denom != 0, "Dzielenie przez 0."
	return skroc([num,denom])
	

# a/b + c/d = a*d + c*d/b*d
def dodaj1(x,y):
	if not czy_liczba(x,y):
		return False
	nnum =  x[0]*y[1] + x[1]*y[0]
	ndenom = x[1]*y[1]
	return nowy_ulamek(nnum,ndenom)
	
# a/b * 
def pomnoz1(x,y):
	if not czy_liczba(x,y):
		return False
	nnum = x[0] * y[0]
	ndenom = x[1] * y[1]
	return nowy_ulamek(nnum,ndenom)

def podziel1(x,y):
	return pomnoz1(x,[y[1],y[0]])

def odejmij1(x,y):
	if not czy_liczba(x,y):
		return False
	nnum = x[0]*y[1] - x[1]*y[0]
	ndenom = x[1]*y[1]
	return nowy_ulamek(nnum,ndenom)	

def dodaj2(x,y):
	if not czy_liczba(x,y):
		return False
	y[0], y[1] = tuple(skroc([x[0]*y[1] + x[1]*y[0], x[1]*y[1]]))

def pomnoz2(x,y):
	if not czy_liczba(x,y):
		return False
	y[0], y[1] = tuple(skroc([x[0]*y[0], x[1]*y[1]]))

def podziel2(x,y):
	if not czy_liczba(x,y):
		return False
	y[0],y[1] = y[1],y[0]
	pomnoz2(x,y)

def odejmij2(x,y):
	if not czy_liczba(x,y):
		return False
	y[0], y[1] = tuple(skroc([x[0]*y[1] - x[1]*y[0], x[1]*y[1]]))

def rownosc(x,y):
	if not czy_liczba(x,y):
		return False
	return nowy_ulamek(x[0],x[1]) == nowy_ulamek(y[0],y[1])
