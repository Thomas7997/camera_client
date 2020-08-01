import serial #import the serial library
from time import sleep #import the sleep command from the time library
import binascii
import io

output_file = open('hygro.txt', 'w') #create a file and allow you to write in it only. The name of this file is hygro.txt

ser = serial.Serial('/dev/ttyAMA0', 115200) #load into a variable 'ser' the information about the usb you are listening. /dev/tty.usbserial.... is the port after plugging in the hygrometer, 9600 is for bauds, it can be diminished
#sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))
count = 0

print(ser.read(10))

while 1:
    #read_byte = ser.read(size=1)
    line = sio.readline()
    print (read_byte, line)

ser.close()
