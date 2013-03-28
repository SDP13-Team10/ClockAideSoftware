import string, time, datetime, serial, re, subprocess, os, usb, re, subprocess, commands

def main():
	keypadPath = "/dev/ttyACM0"
	keypadBaudRate = 9600
	keypad = serial.Serial(keypadPath,keypadBaudRate)
	#print "*\n\n*\n"
	while 1:
		serialInput = keypad.readline()
		keypad.flush()
		print(serialInput)
		#print(keypad.readline())
	
if __name__ == '__main__':main()
