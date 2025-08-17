import serial
import matplotlib.pyplot as plt

ser = serial.Serial()
ser.port = '/dev/ttyS4'
ser.open()
s = "0"
order = 0
wait = 0
save = [[], [], []]
print("Start")

for i in range(500):
  data = ser.read()
  if ord(data) > 13 and wait < 4:
    wait = 0
  if ord(data) > 13 and wait >= 4:
    
    while ord(data) > 13:
      s += chr(ord(data))
      data = ser.read()
    try:
      save[order].append(int(s, 16))
    except:
      pass

    order += 1
    if order > 2:
      order = 0
    s = "0"
  else:
    wait += 1
print(save[0][10])
print(save[1][10])
plt.plot(save[0], label="reference")
plt.plot(save[1], label="measured")
plt.plot(save[2], label="pid")
plt.legend(loc="upper left")
plt.ylim(0, 1500)
plt.savefig("graph.png", dpi=600)