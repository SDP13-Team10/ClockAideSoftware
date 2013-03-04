import string,time,datetime,serial,re,subprocess,os,usb,re,subprocess

#keypad = None #Keypad + LCD display connected to the keypad
#motors = None #Stepper motors + LCD dispaly connected to the arduino

def main():
        initializeHardware()
        speakTime(005,045)

#Initialize the two Arduino boards
def initializeHardware():

		device_re = re.compile("Bus\s+(?P<bus>\d+)\s+Device\s+(?P<device>\d+).+ID\s(?P<id>\w+:\w+)\s(?P<tag>.+)$", re.I)
		df = subprocess.check_output("lsusb", shell=True)
		devices = []
		for i in df.split('\n'):
		    if i:
		    	info = device_re.match(i)
		        if info:
		            dinfo = info.groupdict()
		            dinfo['device'] = '/dev/bus/usb/%s/%s' % (dinfo.pop('bus'), dinfo.pop('device'))
		            devices.append(dinfo)
		print devices

        #Parameters
        keypadBaudRate = 9600
        motorBaudRate = 9600
        keypadDevice = "/dev/ttyACM1"
        motorDevice = "/dev/ttyACM0"

        #Create Serial Object
        #keypad = serial.Serial(keypadDevice, keypadBaudRate)
        #motors = serial.Serial(motorDevice, motorBaudRate)

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
