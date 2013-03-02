import string,time,datetime,serial,re,subprocess

global keypad
global motors

def main():
	print('Hello!')
	getTimeString()

#Initialize the two Arduino boards 
def initializeHardware():

	#Parameters 
	keypadBaudRate = 9600
	motorBaudRate = 9600

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

	#Create Serial Object
	keypad = Serial.serial()
	motors = Serial.serial()

def getTimeString():
	currentTime = datetime.datetime.now()
	#print(currentTime.strftime("%I%M"))
	return currentTime.strftime("%I%M")

if  __name__ =='__main__':main()