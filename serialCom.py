import serial
from time import sleep
from datetime import datetime

# Const variables
PORT = 'com9'
BAUDRATE = 9600
PROTOCOL_SERIAL_COM = ['<','>']

ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM9'
ser.open()
message = ''
data = {
    'Light state': [],
    'Humidity': [],
    'Temperature': [],
    'Sound activity': [],
    'Beehive humidity': [],
    'Beehive temperature': [],
    'Beehive Pressure': []
}
time_registers = []
while(True):
    while (ser.in_waiting > 0):
        message += ser.readline().decode()
    if message.startswith(PROTOCOL_SERIAL_COM[0]) and message.endswith(PROTOCOL_SERIAL_COM[1]):
        new_message = message[1:-1]
        values = new_message.split(';')
        for value, key in zip(values, data):
            data[key].append(float(value))
            time_registers.append(datetime.now().strftime("%m/%d/%Y, %H:%M:%S"))
    print(data)
    message = ''
    ser.close()
    sleep(120)
    ser.open()
