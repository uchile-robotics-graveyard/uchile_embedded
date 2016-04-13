#!/usr/bin/python

import serial
import time
import sys

def calc_checksum(msg):
    chksum = 0
    for m in msg:
        chksum += m
    chksum = ( ~chksum ) % 256
    return chksum

ser = serial.Serial('/dev/ttyUSB1', baudrate = 9600, timeout=1)
# [0xFF, 0xFF, ID, Length, Instruction, Parameters, Checksum]

command = int(sys.argv[1])
print 'Command: ' + str(command)

# ID, Length, Instruction, Parameters
msg = [0x01, 0x03, 0x01, command]
packet = [0xFF,0xFF] + msg + [calc_checksum(msg)]
packet_hex = ['%02x,'%i for i in packet]
print packet_hex
print packet

data = array('B', packet).tostring() # packetStr = ''.join([chr(byte) for byte in packet])

ser.write(data)
