import wave
import numpy as np
from scipy.interpolate import interp1d

wr = wave.open("sample.wav", mode='rb')

bits = []
next_bit = int.from_bytes(wr.readframes(1), byteorder='big')
while(next_bit > 0):
    bits.append(next_bit)
    next_bit = int.from_bytes(wr.readframes(1), byteorder='big')

m = interp1d([min(bits), max(bits)], [0, 255])
print(len(bits))
print(np.ndarray.tolist(np.round(m(bits), 0)))