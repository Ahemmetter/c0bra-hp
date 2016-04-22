## import the serial library
import serial

## Boolean variable that will represent 
## whether or not the arduino is connected
connected = False

## establish connection to the serial port that your arduino 
## is connected to.

ser = serial.Serial("/dev/ttyACM0", 9600)

## loop until the arduino tells us it is ready
while not connected:
    serin = ser.read()
    connected = True

## open text file to store the current 
##gps co-ordinates received from the rover    
text_file = open("position4.txt", 'w')
## read serial data from arduino and 
## write it to the text file 'position.txt'
while 1:
    if ser.inWaiting():
        x=ser.read()
        print(x) 
        text_file.write(x)
        if x=="\n":
             text_file.seek(0)
             text_file.truncate()
        text_file.flush()

## close the serial connection and text file
text_file.close()
ser.close()
