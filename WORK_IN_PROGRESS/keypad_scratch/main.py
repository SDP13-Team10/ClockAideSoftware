import string,time,datetime,serial,re,subprocess,os,usb,re,subprocess,commands

keypad = 0 #Keypad + LCD display connected to the keypad
motors = 0 #Stepper motors + LCD dispaly connected to the arduino

def main():
    	initializeHardware()
	
#Initialize the two Arduino boards
def initializeHardware():

	global keypad
	global motors

     	#Parameters
        keypadBaudRate = 9600
        motorBaudRate = 9600
        #keypadSerial = "64936333037351E0E1E1"
        #motorSerial = "64932343938351119122"
	keypadPath = "/dev/ttyACM2"
	motorPath = "/dev/ttyACM1"

	motors = serial.Serial(motorPath, motorBaudRate)
	keypad = serial.Serial(keypadPath,keypadBaudRate)

	motors.write(getDateTimeString())
	time.sleep(5)
	keypad.write(getDateTimeString())
	time.sleep(5)

        #keypadLocation = getHardwareLocation(keypadSerial)
        #motorLocation = getHardwareLocation(motorSerial)

	#print(motorLocation)

	#if motorLocation != "notConnected":
		#motorPath = "/dev/" + motorLocation
		#motors = serial.Serial(motorPath, motorBaudRate)
		#motors.write(getDateTimeString())
	#else:
		#print("Motor not connected")

	#if keypadLocation != "notConnected":
		#keypadPath = "/dev/" + keypadLocation
		#keypad = serial.Serial(keypadPath,keypadBaudRate)
	#else:
		#print("Keypad not connected")

def putMotorsInSetMode():
	motors.write('2')
	
def getHardwareLocation(serialNumber):
		cmd = "dmesg | grep " + serialNumber + " -A 1 | tail -n 1 | grep -o 'ttyACM[[:digit:]]'"
		cmdOutput = commands.getstatusoutput(cmd)[1]
		#print(cmdOutput)
		if cmdOutput == "/" or cmdOutput == " ":
			return "notConnected"
		else:
			devLocation = cmdOutput
			connectionCheck = "cat /dev/" + devLocation
			connectionCheckOutput = commands.getstatusoutput(connectionCheck)[1]
			#print(connectionCheckOutput)
			if not connectionCheckOutput.strip() or connectionCheckOutput == " " or connectionCheckOutput.strip() == "N":
				return devLocation
			else:
				return "notConnected"

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

def speakCurrentTime():
	hour = datetime.datetime.now().strftime("%H").lstrip('0')
	minute = datetime.datetime.now().strftime("%M").lstrip('0')
	print(hour)
	print(minute)
	speakTime(hour,minute)

def speakTime(hour, minute):

        hourFile = "/home/pi/ClockAideSoftware/VoiceMap/Hours/"+str(hour)+".wav"

        if minute == 0:
                minuteFile="/home/pi/ClockAideSoftware/VoiceMap/Wildcards/oclock.wav"
        else:
                minuteFile="/home/pi/ClockAideSoftware/VoiceMap/Minutes/"+str(minute)+".wav"

        playVoiceMap = "mplayer %s 1>/dev/null 2>&1 " + hourFile + " " + minuteFile
        os.system(playVoiceMap)

if  __name__ =='__main__':main()
