#!/usr/bin/python

import serial
import time
import sys

ser = serial.Serial('/dev/ttyUSB0', baudrate = 9600, timeout=1)

while True:
	print ser.readline()

