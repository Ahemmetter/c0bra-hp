#!/usr/bin/python
import serial								# imports serial package			
import datetime	as dt						# imports datetime package

connected = False							# connection status

ser = serial.Serial("/dev/ttyACM0", 9600, timeout=2)
											# serial connection to
											# Arduino at ttyACM0 with
											# 9600 baud. Timeout is
											# needed for ser.readline()

while not connected:
	# checks if Arduino is connected and ready
    serin = ser.read()
    connected = True
  
text_file = open("log.txt", 'a+')			# opens log.txt in append
											# and read mode. Creates a
											# new file in the current
											# directory, if it doesnt
											# exist yet

while 1:
    if ser.inWaiting():
        x = ser.readline()					# reads one line
        t = dt.datetime.now().strftime('%a, %d.%m.%Y; %H:%M:%S')
											# reads current time
        print x
        print t
        text_file.write(t + ": " + x)
        if x=="\n":
             text_file.seek(0)
             text_file.truncate()
        text_file.flush()

text_file.close()							# closes file and
ser.close()									# serial connection
