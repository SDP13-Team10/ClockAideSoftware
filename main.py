import string,time,datetime,serial,re,subprocess

keypad = None #Keypad + LCD display connected to the keypad
motors = None #Stepper motors + LCD dispaly connected to the arduino

def main():
	print('Hello!')
	getTimeString()

#Initialize the two Arduino boards 
def initializeHardware():

	#Parameters 
	keypadBaudRate = 9600
	motorBaudRate = 9600
	keypadDevice = "/dev/ttyACM2"
	motorDevice = "/dev/ttyACM0"

	#Create Serial Object
	global keypad = Serial.serial(keypadDevice, keypadBaudRate)
	global motors = Serial.serial(motorDevice, motorBaudRate)

#Return the complete time and date in string format
#Required to initialize the stepper motors
def getDateTimeString():
	currentTime = datetime.datetime.now()
	#print(currentTime.strftime("%I%M"))
	return currentTime.strftime("%H, %M, %S, %d, %m, %Y")

#Returns the hour and minute in string format
def getTimeString():
	currentTime = datetime.datetime.now()
	#print(currentTime.strftime("%I%M"))
	return currentTime.strftime("%H, %M")

if  __name__ =='__main__':main()
