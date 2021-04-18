import time

def frame_start():
    print("Frame start")

def frame_update(delta):
    print("Updating", delta)

def frame_end():
    print("Frame end")

last = time.time()
def delta():
    result = time.time() - last
    time = time.time()
    return result

executing = True
def lifecycle():
    while(executing):
        d = delta()
        frame_start()
        frame_update(d)
        frame_end()
        if(d < 0.01): time.sleep(0.01 - d) # Limit speed to 100fps

