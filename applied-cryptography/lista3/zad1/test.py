with open("zad1a.html", "rb") as f:
    h = f.read()

c = b'\x35' * 8

with open("test.html", "wb") as w:
    d = h[:43] + c + h[51:110] + c + h[118:]
    w.write(d)
