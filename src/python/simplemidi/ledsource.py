import time

def solid_pattern(pixels, rgb, brightness):
    l = len(pixels)
    for i in range(0,l,3):
        pixels[i    ] = int(rgb[0] * brightness)
        pixels[i + 1] = int(rgb[1] * brightness)
        pixels[i + 2] = int(rgb[2] * brightness)
