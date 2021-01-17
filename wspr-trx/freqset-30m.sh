#!/bin/sh
cd /home/kenji/wspr-trx
# FT-891, set mode to DATA-U and filter width 350Hz,
# frequency to 10.1387MHz, output power to 5W, then check the status
# (including PTT)
rigctl -m 1036 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_00CED32F-if00-port0 -P RIG -s 38400 - << .EOF.
M PKTUSB 350 
F 10138700
L RFPOWER 0.05
f 
m 
l RFPOWER
t
.EOF.
