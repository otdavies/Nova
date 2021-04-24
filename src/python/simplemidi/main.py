from ledsink import Sink
import time
import math

teensy1 = Sink("192.168.1.69", 8888, [150, 150, 150, 150, 150])
teensy2 = Sink("192.168.1.70", 8888, [180, 180, 180, 180])
teensy3 = Sink("192.168.1.71", 8888, [180, 180, 180, 180])
teensy4 = Sink("192.168.1.72", 8888, [180, 180, 180, 180])

# acrylic left -> 72, 1
# acrylic left -> 71, 2

#while(True):
# val = int(math.sin(((time.time() % 4) * 0.25) * math.pi) * 255)
teensy1.setColorAll([255, 50, 0])
teensy2.setColorAll([255, 255, 255])
teensy3.setColorAll([255, 255, 255])
teensy4.setColorAll([255, 255, 255])

time.sleep(0.1)
teensy3.setColor(2, [10, 180, 0])
teensy4.setColor(1, [10, 0, 180])
teensy1.setColor(4, [255, 255, 255])
 
# while True:
#     solid_pattern(msg, )
#     send(msg)
#     sock.recvfrom(1)
