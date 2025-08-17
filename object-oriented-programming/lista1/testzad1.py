from zadanie1 import *
from random import randint

def f(L,x,y):
	for i in L:
		przesun(i,x,y)
	return L

def r():
	return randint(-10,10)


if __name__ == "__main__":
	kwadraty = []
	trojkaty = []
	okregi   = []
	print("Utworzenie 3 nowych kwadratów")
	for i in range(3):
		kwadraty.append(new_square(r(),r(),r()))
		print("Kwadrat",i,kwadraty[i][1],"Pole:",pole(kwadraty[i]))
	print("Suma pól:",sumapol(kwadraty,3))
	print("Utworzenie 3 nowych okręgów")
	for i in range(3):
		c = randint(0, 20)
		okregi.append(new_circle(r(),r(),c))
		print("Okrąg",i,okregi[i][1],"Pole:",pole(okregi[i])) 
	print("Suma pól:",sumapol(okregi,3))
	print("Utworzenie 3 nowych trójkątów")
	for i in range(3):
		trojkaty.append(new_triangle(r(),r(),r(),r()))
		print("Trójkąt",i,trojkaty[i][1],"Pole:",pole(trojkaty[i])) 
	print("Suma pól:",sumapol(trojkaty,3))
	a = r()
	b = r()
	print(f"Przesuń kwadraty o wektor ({a},{b})")
	f(kwadraty,a,b)
	for i in range(3):
		print("Kwadrat",i,kwadraty[i][1],"Pole:",pole(kwadraty[i]))
	print("Suma pól:",sumapol(kwadraty,3))	
