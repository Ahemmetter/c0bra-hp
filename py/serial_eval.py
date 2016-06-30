#!/usr/bin/python

# asks user for input file name (default is 'log').
input_file = str(raw_input("input file: ") or "log")
inFile = open(input_file + ".txt")
outFile = open(input_file + "_results.txt", "w")
arr = []
arr_new = []
counter = 0
removelist = [0, 1, 3, 4, 5, 7, 8, 9, 11]

for line in inFile:
    # reads each line, transforms it into a list and removes all
    # elements which are not values or time stamps.
    arr = line.split()
    arr_new = [v for i, v in enumerate(arr) if i not in removelist]
    print arr_new           # prints current line contents
    if len(arr_new) == 3:
        outFile.write(arr_new[0][:-1] + ", ")
        outFile.write(arr_new[1] + ", ")
        outFile.write(arr_new[2])
        outFile.write("\n")
        counter += 1

print counter               # prints total number of lines
inFile.close()
outFile.close()
