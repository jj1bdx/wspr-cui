#!/bin/sh
#
# Running this script will send a prebuilt WAV file as a WSPR beacon
# Set rig to PKTUSB mode
#
# FT-891, 10.1387MHz, 5W, enable TX
rigctl -m 136 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_00CED32F-if00-port0 -P RIG -s 38400 - << .EOF.
f 
m 
l RFPOWER
F 10138700
L RFPOWER 0.02
T 1
f 
m 
l RFPOWER
t
.EOF.
# Send sound
aplay -D hw:4,0 ./wsprsimwav.wav
# FT-891, disable TX, set back to 100W
rigctl -m 136 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_00CED32F-if00-port0 -P RIG -s 38400 - << .EOF.
T 0
L RFPOWER 0.39
f 
m 
l RFPOWER
t
.EOF.
