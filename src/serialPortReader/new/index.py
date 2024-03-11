# Python code transmits a byte to Arduino /Microcontroller
import serial
import time
from matplotlib import pyplot as plt
SerialObj = serial.Serial('COM5') # COMxx  format on Windows
                  # ttyUSBx format on Linux
SerialObj.baudrate = 10000  # set Baud rate to 9600
SerialObj.bytesize = 8   # Number of data bits = 8
SerialObj.parity  ='N'   # No parity
SerialObj.stopbits = 1   # Number of Stop bits = 1
# time.sleep(3)
SerialObj.write(b'A')    #transmit 'A' (8bit) to micro/Arduino

while True:
    ReceivedString = SerialObj.readline()
    ReceivedString = ReceivedString.decode("utf-8")
    ReceivedString = ReceivedString.replace("\r","")
    ReceivedString = ReceivedString.replace("\n","")

    ReceivedString = ReceivedString.split(" ")
    spacing = []
    for i in range(0,6):
        ReceivedString[i] = int(ReceivedString[i])
        print(ReceivedString[i])
    print(ReceivedString[6])
    plt.plot(ReceivedString)
    plt.legend() 
    plt.show()
SerialObj.close() 