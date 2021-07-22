#!/usr/bin/python
# -*- coding: utf-8 -*-

# Description: Convert .txt NS-3 building file to .csv file
#
# Author: Lahis Almeida
#
# To run:
# $ python buildings_to_csv.py --input "buildings.txt" --output "buildings.csv"
# $ python buildings_to_csv.py 


# libs
import csv
import argparse
import sys


# dataset colunms name
building_colunms_names  = [
    "building_id",
    "boundaries_xMin",
    "boundaries_yMin",
    "boundaries_xMax",
    "boundaries_yMax"
]


#---- extract buildings settings of ns3 txt result file
def get_buildings_settings(buildings_filename):
    
    # list of all rows of buildings_settings
    file_rows = []

    # open file in a read mode
    file = open(buildings_filename, 'r')

    for line in file.readlines():
        
        # create a str without \n and " "
        line_str = line.strip().split(" ")
        
        # create a list with ID and building dimensions
        row_list = []
        for i in line_str:
            if i.find(',') != -1:
                a = i.split(",")
                row_list.append(a[0])
                row_list.append(a[1])
            elif i.isnumeric():
                row_list.append(i)
       
        
        file_rows.append(row_list)

    return file_rows

#---- create a csv file
def create_csv(title):
    with open(title,"w+") as file:
        writer = csv.writer(file, delimiter=",")
        writer.writerow(building_colunms_names)


#---- add row on csv file
def add_csv_rows(b_settings, title):
    for colunm in b_settings:
        if len(colunm) == len(building_colunms_names):
            with open(title,"a") as file:
                writer = csv.writer(file, delimiter=",")
                writer.writerow([float(x) for x in colunm])
    




#----------- MAIN PROGRAM -----------

if __name__ == '__main__':

    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--input",
        type = str,
        default = 'buildings.txt',
        help = "File name of the txt buildings file"
    )

    parser.add_argument(
        "--output",
        type = str,
        default = 'buildings.csv',
        help = "File name of the csv buildings file"
    )
       

    args = parser.parse_args()


    b_settings = get_buildings_settings(args.input)

    # for i in b_settings:
    #     print (i)

    # create a csv file with b_settings lists
    create_csv(args.output)
    add_csv_rows(b_settings, args.output)

    