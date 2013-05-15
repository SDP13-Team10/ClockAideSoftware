#! /usr/bin/env python
import string,time,datetime,serial,re,subprocess,os,usb,re,subprocess,commands
from subprocess import Popen, PIPE

keypad = serial.Serial() #Keypad + LCD display connected to the keypad
motors = serial.Serial() #Stepper motors + LCD dispaly connected to the arduino

def main():
        #initializeHardware()
        p = Popen(['python', './keypad.py'], stdin=PIPE, stdout=PIPE, stderr=PIPE)

#Initialize the two Arduino boards    
def getHardwareLocation(serialNumber):
        cmd = "dmesg | grep " + serialNumber + " -A 1 | tail -n 1 | grep -o 'ttyACM[[:digit:]]'"
        cmdOutput = commands.getstatusoutput(cmd)[1]
        if cmdOutput == "/" or cmdOutput == " ":
            return "notConnected"
        else:
            devLocation = cmdOutput
            connectionCheck = "cat /dev/" + devLocation
            connectionCheckOutput = commands.getstatusoutput(connectionCheck)[1]
            #print(connectionCheckOutput)
            if connectionCheckOutput == "/" or connectionCheckOutput == " " or connectionCheckOutput.strip() == "N":
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
