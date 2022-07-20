import Adafruit_BBIO.PWM as PWM
import time

pin = "P8_13"

# PWM.start(pin, 50, 24000000)
PWM.start(pin, 50, 1)

time.sleep(3000)

PWM.stop(pin)
PWM.cleanup()