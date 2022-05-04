import serial
from time import sleep
from datetime import datetime
from JsonManager import JsonManager

# Const variables
PROTOCOL_SERIAL_COM = ['<','>']

ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM8'
# ser.port = '/dev/tty/ACM0'
ser.open()
message = ''
jmanager = JsonManager()
data = jmanager.readJson('data.json')
time_registers = []
while(True):
    while (ser.in_waiting > 0):
        message = ser.readline().decode().replace('\r\n', '')
    if message.startswith(PROTOCOL_SERIAL_COM[0]) and message.endswith(PROTOCOL_SERIAL_COM[1]):
        new_message = message[1:-1]
        message = ''
        values = new_message.split(';')
        print(values)
        for value, key in zip(values, data):
            data[key].append(float(value))
            jmanager.updateJson('data.json', data)
            time_registers.append(datetime.now().strftime("%m/%d/%Y, %H:%M:%S"))
    message = ''
    sleep(5)
