import serial #import the serial library
from time import sleep #import the sleep command from the time library
import binascii

output_file = open('hygro.txt', 'w') #create a file and allow you to write in it only. The name of this file is hygro.txt

ser = serial.Serial("/dev/bus/usb/001/013", 9600) #load into a variable 'ser' the information about the usb you are listening. /dev/tty.usbserial.... is the port after plugging in the hygrometer, 9600 is for bauds, it can be diminished
count = 0
while 1:
    read_byte = ser.read(size=1)
    print (read_byte)