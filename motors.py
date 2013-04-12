import string,time,datetime,serial,re,subprocess,os,usb,re,subprocess,commands,sys

motorBaudRate = 9600

while True:
    counter += 1
    sys.stdin.readline()
    sys.stdout.write('Serial from com1 is %d\n' % counter)
    sys.stdout.flush()