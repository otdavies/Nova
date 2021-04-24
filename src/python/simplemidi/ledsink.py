import ctypes
import struct
import socket

class Sink:
    def __init__(self, address, port, leds):
        self.address = address
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.leds = leds
        self.buffers = {}

    def setColorAll(self, color):
        for i in range(len(self.leds)):
            self.setColor(i, color)

    def setColor(self, id, color):
        if(id < len(self.leds)):
            self.buffers[id] = bytearray([id])
            self.buffers[id].extend(color * self.leds[id])
            self.send(id)

    def setPixels(self, id, pixels):
        if(id < len(self.leds)):
            self.buffers[id] = bytearray([id])
            self.buffers[id].extend(pixels)
            self.send(id)

    def send(self, id):
        msg = self.buffers[id]
        # print(self.buffers[id])
        self.sock.sendto(msg, (self.address, self.port))
        # print("Waiting on recv")
        # # self.sock.recvfrom(1)
        # print("Got it")

