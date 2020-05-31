

import serial
from time import sleep
port = 'COM42'
baud = 115200
ser = serial.Serial(port, baud)
import random
tps = random.randint(1,100)
speed = random.randint(1,160)
rpm = random.randint(1600,10000)
atf = random.randint(100,600)
bf = random.randint(0,255)
writestring = "AA,{%d},{%d},\n"
try:
    while(1):
        tps = random.randint(1,100)
        writestring = 'AA,11,'+str(tps)+',\n'
        ser.write(writestring.encode())
        sleep(0.065)

        speed = random.randint(1,160)
        writestring = 'AA,0D,'+str(speed)+',\n'
        ser.write(writestring.encode())
        sleep(0.065)

        rpm = random.randint(1600,10000)
        writestring = 'AA,0C,'+str(rpm)+',\n'
        ser.write(writestring.encode())
        sleep(0.065)

        atf = random.randint(100,600)
        writestring = 'AA,10,'+str(atf)+',\n'
        ser.write(writestring.encode())
        sleep(0.065)

        bf = random.randint(0,255)
        writestring = 'AA,33,'+str(bf)+',\n'
        ser.write(writestring.encode())
        sleep(0.065)

except KeyboardInterrupt:
    ser.close()
