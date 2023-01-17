import serial
from datetime import datetime
from os import path
import time
import csv

MEASURE_TIME = 10 #minutes
value = input("Please enter file name (e.g., 10) for the measurement\n")
wifi_file = 'WiFi/wifi'+value+'.csv'
wifi_head = ['timestamp','SSID','RSSI','CH']
lora_file = 'Lora250/lora'+value+'.csv' #edit lora path based on freq
lora_head = ['timestamp','ID','RSSI','SNR']

if (path.exists(wifi_file)):
    f = open(wifi_file, 'a', encoding='UTF8', newline='')
    write_wifi = csv.writer(f)
else:
    f = open(wifi_file, 'w', encoding='UTF8', newline='')
    write_wifi = csv.writer(f)
    write_wifi.writerow(wifi_head)

if (path.exists(lora_file)):
    l = open(lora_file, 'a', encoding='UTF8', newline='')
    write_lora = csv.writer(l)
else:
    l = open(lora_file, 'w', encoding='UTF8', newline='')
    write_lora = csv.writer(l)
    write_lora.writerow(lora_head)

#ser = serial.Serial('/dev/tty.usbserial-01EED935',115200)
ser = serial.Serial('/dev/tty.usbserial-01ED9D5F',115200)
ser.flushInput()

TIME_LIMIT = time.time() + 60*MEASURE_TIME 

while (time.time()<=TIME_LIMIT):
    try:
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes.decode()
        print(str(datetime.now())+":"+decoded_bytes)
        data = decoded_bytes.split(":")
        if data[0]=='WIFI':
            data[3] = int(data[3])
            print("WriteWiFi")
            data[0]=datetime.now()
            write_wifi.writerow(data)
        elif data[0]=='LORA':
            data[3] = float(data[3])
            print("WriteLoRa")
            data[0]=datetime.now()
            write_lora.writerow(data)
        else:
            print("Uknown Serial Message")

    except:
        print("Keyboard Interrupt")
        break



