#!/bin/sh
cd /home/kenji/wspr-trx
# IC-705
#  set frequency to 21.0946MHz
#  set mode to USB-D
#  set filter to 300Hz
#  set actual output power to 5W (50%)
#  set preamp to disable (0)
#  disable transmission (0)
# then check the status including PTT
rigctl -m 3085 -r /dev/serial/by-id/usb-Icom_Inc._IC-705_IC-705_31002769-if00 -P RIG -s 19200 - << .EOF.
F 21094600
M PKTUSB 300
L RFPOWER 0.510
L PREAMP 0
T 0
f 
m 
l RFPOWER
l PREAMP
t
.EOF.
