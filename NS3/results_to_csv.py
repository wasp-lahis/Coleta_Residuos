#!/usr/bin/python
# -*- coding: utf-8 -*-

# Description: Convert NS-3 .txt to .csv file
#
# Author: Lahis Almeida
#
# To run:
# $ python results_to_csv.py --input "resultados.txt" --output "results.csv"
# $ python results_to_csv.py 


# libs
import csv
import argparse
import sys


# dataset colunms name
network_colunms_names  = [
    "ed_id",
    "ed_x",
    "ed_y",
    "ed_z",
    "sf",
    "gw_id",
    "gw_x",
    "gw_y",
    "gw_z",
    "distance"
]


#---- extract network settings of ns3 txt result file
def get_network_settings(network_filename):
    
    # list of all rows of network_settings
    file_rows = []

    # open file in a read mode
    file = open(network_filename, 'r')

    for line in file.readlines():
        
        # create a str without \n and " "
        line_str = line.strip().split(",")  
        file_rows.append(line_str)

    return file_rows

#---- create a csv file
def create_csv(title):
    with open(title,"w+") as file:
        writer = csv.writer(file, delimiter=",")
        writer.writerow(network_colunms_names)


#---- add row on csv file
def add_csv_rows(b_settings, title):
    for colunm in b_settings:
        if len(colunm) == len(network_colunms_names):
            with open(title,"a") as file:
                writer = csv.writer(file, delimiter=",")
                writer.writerow([float(x) for x in colunm])
    




#----------- MAIN PROGRAM -----------

if __name__ == '__main__':

    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--input",
        type = str,
        default = 'results.txt',
        help = "File name of the txt results file"
    )

    parser.add_argument(
        "--output",
        type = str,
        default = 'results.csv',
        help = "File name of the csv results file"
    )
       

    args = parser.parse_args()


    n_settings = get_network_settings(args.input)

    for i in n_settings:
        print (i)

    # create a csv file with n_settings lists
    create_csv(args.output)
    add_csv_rows(n_settings, args.output)

    
