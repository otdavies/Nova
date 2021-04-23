import socket
import struct
import ctypes
import time

LED_COUNT = 480
MESSAGE_BYTES = LED_COUNT * 3
buffer = (ctypes.c_byte * MESSAGE_BYTES)()

UDP_IP = "10.0.0.69"
UDP_PORT = 8888
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send(message):
    sock.sendto(message, (UDP_IP, UDP_PORT))

def pack(data):
    return struct.pack('%sB' % len(data), *data)

x = 0
def pattern(pixels):
    l = len(pixels)
    for i in range(0,l,3):
        pixels[i] = 0
        pixels[i + 1] = 10 - min(10, abs(x - i))
        pixels[i + 2] = 0

    x = (x + 1) % 120

msg = pack([255,254,253] * 150)
count = 0
while True:
    print(count)
    
    count += 1
    send(msg)
    sock.recvfrom(1)
