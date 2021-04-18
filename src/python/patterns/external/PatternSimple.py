
from boilderplate.properties import Properties

class PatternSimple(Pattern):
    def __init__(self, pixels, properties):
        super().__init__(pixels)
        self.speed = properties.speed
        self.width = properties.width

    def mutate(self, tick):
        for xe4 in range(len(self.pixels)): 
            self.pixels 
    