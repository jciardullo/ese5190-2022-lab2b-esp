"""
Blink example for boards with ONLY a NeoPixel LED (e.g. without a built-in red LED).
Includes QT Py and various Trinkeys.
Requires two libraries from the Adafruit CircuitPython Library Bundle.
Download the bundle from circuitpython.org/libraries and copy the
following files to your CIRCUITPY/lib folder:
* neopixel.mpy
* adafruit_pixelbuf.mpy
Once the libraries are copied, save this file as code.py to your CIRCUITPY
drive to run it.
"""
import board
import analogio
import neopixel
import time

adc = analogio.AnalogIn(board.A3)
pixel = neopixel.NeoPixel(board.NEOPIXEL, 1)
pixel.fill((255, 255, 255))

while True:
    pixel.brightness = adc.value / 65535
    #print(adc.value)
    time.sleep(0.1)
