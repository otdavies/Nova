import socket
import struct
import ctypes
import time
import ui

LED_COUNT = 480
MESSAGE_BYTES = LED_COUNT * 3
buffer = (ctypes.c_byte * MESSAGE_BYTES)()

UDP_IP = "10.0.0.69"
UDP_PORT = 8888
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send(message):
    sock.sendto(message, (UDP_IP, UDP_PORT))

def convert(data):
    return struct.pack('%sB' % len(data), *data)

# msg = convert([255,254,253] * 480)
# count = 0
# while True:
#     print(count)
#     count += 1
#     send(msg)
#     sock.recvfrom(1)
