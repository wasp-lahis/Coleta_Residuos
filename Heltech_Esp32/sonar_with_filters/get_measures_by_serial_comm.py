#!/usr/bin/python
# -*- coding: utf-8 -*-

#---- get values from serial and feed csv file

# libs
import serial
import time
import csv

#---- open port of serial communication
def open_serial_communication(port, baud_rate):
    try:
        ser = serial.Serial(port, baud_rate,timeout = 5)
        ser.flushInput()    # limpa buffer
        time.sleep(1)
        return ser
    except SerialException:
        print("ERROR: Check serial port")
        exit()


#---- create a csv file
def create_csv(title):
    with open(title,"w+") as file:
        writer = csv.writer(file, delimiter=",")
        writer.writerow(features)


features  = ["raw","mov_average", "mean","median","time"]
cm_expected = 600
csv_title = "sonar_data_" + str(cm_expected) + ".csv"

test_duration_time = 5 * 60 # min
init_time = time.time()
current_time = time.time()

create_csv(csv_title)
serial_comm = open_serial_communication('/dev/ttyUSB0', 115200)

while (current_time - init_time < test_duration_time):
    try:
        ser_bytes = serial_comm.readline()
        decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
        decoded_bytes = decoded_bytes.split()

        data = [float(x) for x in decoded_bytes]
        data.append(current_time)
        print(data)

        if len(data) == len(features):
            with open(csv_title,"a") as file:
                writer = csv.writer(file, delimiter=",")
                writer.writerow(data)
        current_time = time.time()
    except:
        print("ERROR or Keyboard Interrupt")
        break

exit()
