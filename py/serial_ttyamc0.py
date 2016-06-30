#!/usr/bin/python
import serial           # imports serial package
import datetime	as dt   # imports datetime package

connected = False       # connection status
# asks user for serial port name. By default it is ttyACM0 for linux
port = str(raw_input("port name (ttyACM*): ") or "ttyACM0")
# serial connection is grabbed with 9600 baud. Timeout is needed for
# ser.readline()
ser = serial.Serial("/dev/" + port, 9600, timeout=2)

while not connected:
    # checks if Arduino is connected and ready
    serin = ser.read()
    connected = True
    print connected

# opens a log.txt file in the current directory in "append" mode
text_file = open("log.txt", 'a+')

while 1:
    if ser.inWaiting():
        x = ser.readline()              # reads one line
        # reads current time
        t = dt.datetime.now().strftime('%a, %d.%m.%Y; %H:%M:%S')
        print x, t
        text_file.write(t + ": " + x)   # writes time stamp and values
        if x=="\n":
             text_file.seek(0)
             text_file.truncate()
        text_file.flush()

text_file.close()                       # closes file and
ser.close()                             # serial connection
