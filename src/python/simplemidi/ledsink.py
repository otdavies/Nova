import ctypes
import struct
import socket
import numpy as np

class ledsink:
    def __init__(self, address, port, leds):
        self.address = address
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.leds = leds
        self.mhz = 12
        self.buffer = []
        for num in leds:
            buffer.append(ctypes.c_byte * num * 3)

    def setMHZ(self, mhz):
        self.mhz = mhz
        self.sock.sendto([self.Byte(255)] + [self.Byte(mhz)], (self.address, self.port))

    def setToColor(self, id, color):
        self.buffer[id] = color * len(self.buffer[id])

    def setToPixels(self, id, pixels):
        self.buffer[i] = pixels
        self.send()

    def send(self, id):
        msg = pack(self.buffer[id])
        self.sock.sendto([self.Byte(id)] + msg, (self.address, self.port))

    def pack(self, data):
        return struct.pack('%sB' % len(data), *data)

    def Byte(self, num):
        return int(num).to_bytes(1, byteorder="big")



