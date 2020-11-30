#!/usr/bin/python
# -*- coding: utf-8 -*-

# Description: Get values from serial and feed a csv file
#
# Author: Lahis Almeida
#
# >> python get_measures.py --usb_port "/dev/ttyUSB0" --cm_distance 120


# libs
import serial
import time
import csv
import argparse
import sys

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
		writer.writerow(colunms_features)


#---- get distance measures
def get_measures(usb_port, cm_distance):
	port = usb_port
	cm_expected = int(cm_distance)
	csv_title = "data_" + str(cm_expected) + ".csv"

	# test_duration_time = 5 * 60 # min
	# init_time = time.time()
	current_time = time.time()

	create_csv(csv_title)
	serial_comm = open_serial_communication(port, 115200)

	#while (current_time - init_time < test_duration_time):
	while (True):
		try:
			ser_bytes = serial_comm.readline()
			decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
			decoded_bytes = decoded_bytes.split()

			data = [float(x) for x in decoded_bytes]
			data.append(current_time)
			print(data)

			if len(data) == len(colunms_features):
				with open(csv_title,"a") as file:
					writer = csv.writer(file, delimiter=",")
					writer.writerow(data)
			current_time = time.time()
		except:
		 	print("ERROR or Keyboard Interrupt")
		 	break




colunms_features  = [
    "sonar_raw",
    "sonar_mean",
    "sonar_median",
    "lidar_raw",
    "time"
]


#----------- MAIN PROGRAM -----------

if __name__ == '__main__':

	parser = argparse.ArgumentParser()

	parser.add_argument(
		"--usb_port", type=str,
		help="Usb port to use in serial communication"
	)
	parser.add_argument(
		"--cm_distance", type=int,
		help="Distance expected to be measure"
	)
	# parser.add_argument(
	#     "--max_evals", type=int, default=500,
	#     help="Maximum number of evaluations"
	# )
	

	args = parser.parse_args()


	get_measures(
		usb_port=args.usb_port,
		cm_distance=args.cm_distance,
	)


	

	
