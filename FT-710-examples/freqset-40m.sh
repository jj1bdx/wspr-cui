#!/bin/sh
cd /home/kenji/wspr-trx
# FT-710
#  set frequency to 7.0386MHz
#  set mode to DATA-U
#  (not required: set HF MAX output power to 5W / send_raw ; EX030401005;)
#  set actual output power to 5W / send_raw ; PC005;
#  set filter to narrow (300Hz) / send_raw ; NA01;
# then check the status including PTT
rigctl -m 1049 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_011BD871-if00-port0 -P RIG -s 38400 - << .EOF.
F 7038600
M PKTUSB 2400
send_raw ; PC005;
send_raw ; NA01;
f 
m 
send_raw ; PC;
send_raw ; NA0;
t
.EOF.
