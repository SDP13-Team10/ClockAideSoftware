import string,time,datetime,serial

global keypad
global motors

def main():
	print('Hello!')
	getTimeString()

#Initialize the two Arduino boards 
def initializeHardware():

	#Parameters 
	keypadDeviceLocation = "/dev/ttyACM0"
	motorDeviceLocation = "/dev/ttyACM1"
	keypadBaudRate = 9600
	motorBaudRate = 9600

	#Create Serial Object
	keypad = Serial.serial()
	motors = Serial.serial()

def getTimeString():
	currentTime = datetime.datetime.now()
	#print(currentTime.strftime("%I%M"))
	return currentTime.strftime("%I%M")

if  __name__ =='__main__':main()