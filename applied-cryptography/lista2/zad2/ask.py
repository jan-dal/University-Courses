from wyrocznia import tell

cipher = bytes.fromhex(open("cipher").read())
def xor(a, b, c):
    return bytes(a ^ b ^ c for a, b, c in zip(a, b, c))

def reconstruct(cipher):

    message = bytearray(16)
    for j in range(16):
        cbyte = cipher[15-j:16]
        for i in range(256):
            pad = int.to_bytes(j+1) * (j+1)
            part = xor(cbyte, int.to_bytes(i) + message[16-j:], pad)
            ok_pad = tell(cipher[:15-j] + part + cipher[16:])
             
            if ok_pad:
                message[15-j] = i 
                if i != j+1: break

    print(message.decode("ascii"))

reconstruct(cipher)
