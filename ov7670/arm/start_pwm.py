import Adafruit_BBIO.PWM as PWM
import time

ADDR = 0x42
REG_GAIN = 0x00
REG_BLUE = 0x01
REG_RED = 0x02
REG_COM1 = 0x04
REG_VREF = 0x03
REG_COM4 = 0x0d
REG_COM5 = 0x0e
REG_COM6 = 0x0f
REG_AECH = 0x10
REG_CLKRC = 0x11
REG_COM7 = 0x12
COM7_RGB = 0x04
REG_COM8 = 0x13
COM8_FASTAEC = 0x80    // Enable fast AGC/AEC
COM8_AECSTEP = 0x40    // Unlimited AEC step size
COM8_BFILT = 0x20    // Band filter enable
COM8_AGC = 0x04    // Auto gain enable
COM8_AWB = 0x02    // White balance enable
COM8_AEC = 0x0
REG_COM9 = 0x14
REG_COM10 = 0x15
REG_COM14 = 0x3E
REG_SCALING_DCWCTR = 0x72
REG_SCALING_PCLK_DIV = 0x73
REG_COM11 = 0x3B
COM11_NIGHT = 0x80
COM11_NMFR = 0x60
COM11_HZAUTO = 0x10
COM11_50HZ = 0x08
COM11_EXP = 0x0
REG_TSLB = 0x3A
REG_RGB444 = 0x8C
REG_COM15 = 0x40
COM15_RGB565 = 0x10
COM15_R00FF = 0xc0
REG_HSTART = 0x17
REG_HSTOP = 0x18
REG_HREF = 0x32
REG_VSTART = 0x19
REG_VSTOP = 0x1A
REG_COM3 = 0x0C
REG_MVFP = 0x1E
REG_COM13 = 0x3d
COM13_UVSAT = 0x40
SCALING_DCWCTR = 0x72
SCALING_PCLK_DIV = 0x73
REG_BD50MAX = 0xa5
REG_BD60MAX = 0xab
REG_AEW = 0x24
REG_AEB = 0x25
REG_VPT = 0x26
REG_HAECC1 = 0x9f
REG_HAECC2 = 0xa0
REG_HAECC3 = 0xa6
REG_HAECC4 = 0xa7
REG_HAECC5 = 0xa8
REG_HAECC6 = 0xa9
REG_HAECC7 = 0xaa
REG_COM12 = 0x3c
REG_GFIX = 0x69
REG_COM16 = 0x41
COM16_AWBGAIN = 0x08
REG_EDGE = 0x3f
REG_REG76 = 0x76
ADCCTR0 = 0x20



pin = "P8_34"

# PWM.start(pin, 50, 24000000)
# PWM.start(pin, 50, 2400000)
PWM.start(pin, 50, 1000000)

while True:
    time.sleep(1)

PWM.stop(pin)
PWM.cleanup()