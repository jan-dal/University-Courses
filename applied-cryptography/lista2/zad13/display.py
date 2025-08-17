import matplotlib.pyplot as plt
import sys

f = open(sys.argv[1], "rb")

data = f.read()
f.close()

img = []
for i in range(256):
    row = []
    for j in range(256):
        pixel = []
        for k in range(4):
            val = data[1024*i+4*j+k]
            if k != 3:
                pixel.append(val)
        row.append(pixel)
    img.append(row)

plt.imshow(img, interpolation='none')
plt.show()
