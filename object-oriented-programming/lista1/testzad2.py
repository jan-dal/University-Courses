from zadanie2 import *
from random import randint 

def r():
	return randint(-200,200)

def test(a,b,c,d):
	f = nowy_ulamek(a,b)
	g = nowy_ulamek(c,d)
	print(f"f = {f}, g = {g}")
	t1 = dodaj1(f,g)
	a = g[:]
	dodaj2(f,a)
	print(f"f + g = {t1}, f + g = {a}")
	assert rownosc(t1,a)
	a = g[:]
	t2 = odejmij1(f,g)
	odejmij2(f,a)
	print(f"f - g = {t2}, f - g = {a}")
	assert rownosc(t2,a)
	a = g[:]
	t3 = podziel1(f,g)
	podziel2(f,a)
	print(f"f/g = {t3}, f/g = {a}")
	assert rownosc(t3,a)
	a = g[:]
	t4 = pomnoz1(f,g)
	pomnoz2(f,a)
	print(f"f*g = {t4}, f*g = {a}")
	assert rownosc(t4,a)


if __name__ == "__main__":
	print("Losowe ułamki")
	test(r(),r(),r(),r())
	a = [int(k) for k in input("Podaj własne ułamki(4 liczby po spacji): ").split()]
	test(*a)
	print("Ok")
