import string,time,datetime,serial,re,subprocess,os,usb,re,subprocess,commands,sys

keypadBaudRate = 9600
keypadSerial = "64936333037351E0E1E1"
keypadLocation = "/dev/ttyACM0"

keypad = serial.Serial(keypadLocation,keypadBaudRate) #Keypad + LCD display connected to the keypad

currentTime = datetime.datetime.now()
currentTime = currentTime.strftime("%H, %M, %S, %d, %m, %Y")
keypad.write(currentTime)

while True:
    mainInput = sys.stdin.readline()
    if mainInput is "RX":
    	sys.stdout.write(serial.readline())
    else:
    	serial.write(mainInput)
    sys.stdin.flush()
    sys.stdout.flush()