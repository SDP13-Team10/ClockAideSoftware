import string,time,datetime,serial,re,subprocess,os,usb,re,subprocess,commands

#keypad = None #Keypad + LCD display connected to the keypad
#motors = None #Stepper motors + LCD dispaly connected to the arduino

def main():
    	initializeHardware()
        speakTime(005,045)

#Initialize the two Arduino boards
def initializeHardware():

     	#Parameters
        keypadBaudRate = 9600
        motorBaudRate = 9600
        keypadSerial = "64936333037351E0E1E1"
        motorSerial = "64932343938351119122"

        keypadLocation = getHardwareLocation(keypadSerial)
        getHardwareLocation(motorSerial)

        #Create Serial Object
        #keypad = serial.Serial(keypadDevice, keypadBaudRate)
        #motors = serial.Serial(motorDevice, motorBaudRate)

def getHardwareLocation(serialNumber):
		cmd = "dmesg | grep " + serialNumber + " -A 1 | tail -n 1 | grep -o 'ttyACM[[:digit:]]'"
		cmdOutput = commands.getstatusoutput(cmd)[1]
		if cmdOutput == "" or cmdOutput == " ":
			return "notConnected"
		else:
			devLocation = cmdOutput
			connectionCheck = "cat /dev/" + devLocation
			connectionCheckOutput = commands.getstatusoutput(connectionCheck)[1]
			if connectionCheckOutput = "" or connectionCheckOutput = " ":
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

def speakTime(hour, minute):

        hourFile = "/home/pi/ClockAideSoftware/VoiceMap/Hours/"+str(hour)+".wav"

        if minute == 0:
                minuteFile="/home/pi/ClockAideSoftware/VoiceMap/Wildcards/oclock.wav"
        else:
                minuteFile="/home/pi/ClockAideSoftware/VoiceMap/Minutes/"+str(minute)+".wav"

        playVoiceMap = "mplayer %s 1>/dev/null 2>&1 " + hourFile + " " + minuteFile
        os.system(playVoiceMap)

if  __name__ =='__main__':main()
