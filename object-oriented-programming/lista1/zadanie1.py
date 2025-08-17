# Rozszerzenie typu o trapez wymaga zmodyfikowania typfig, 
# dodania nowej funkcji tworzącej oraz dodania sposobu obliczania pola

class typfig:
	trojkat, kolo, kwadrat = range(3)
#	trojkat, kolo, kwadrat, trapez = range(4)

def pole(f):
	if f[0] == typfig.kwadrat:
		return abs(f[1][0][0] - f[1][2][0])**2 
	if f[0] == typfig.trojkat:
		return abs(f[1][0][0]-f[1][1][0])*abs(f[1][2][1])/2
	if f[0] == typfig.kolo:
		return 3.14*f[1][1]**2
#   if f[0] == typfig.trapez:
#		return wzor na pole trapezu

def przesun(f,x,y):
	for i in range(len(f[1])):
		try:
			f[1][i] = (f[1][i][0]+x,f[1][i][1]+y)
		except:
			continue
			
def sumapol(t,size):
	suma = 0
	for i in range(size):
		suma += pole(t[i])
	return suma

def new_square(a, b, c):
	assert c != 0, "Szerokość nie może być równa 0."
	return [typfig.kwadrat,[(a,b),(a+c,b),(a+c,b+c),(a,b+c)]]
	
def new_triangle(a,b,c,h):
	assert h != 0, "Wysokość nie może być równa 0."
	return [typfig.trojkat,[(a,0),(b,0),(c,h)]]
	
def new_circle(a,b, r):
	assert r >= 0, "Promień nie może być ujemny."
	return [typfig.kolo,[(a,b),r]]

#def new_trapezoid(*args):
#	wyjątki
#	return [typfig.trapez,[współrzędne]]
