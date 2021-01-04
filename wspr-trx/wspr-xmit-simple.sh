#!/bin/sh
cd /home/kenji/wspr-trx
# Turn PTT on by CAT
rigctl -m 1036 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_00CED32F-if00-port0 -P RIG -s 38400 T 1 t
# Play back the recorded audio data for WSPR
aplay -D hw:3,0 ./jj1bdx-wspr-1536hz.wav
# Turn PTT off by CAT
rigctl -m 1036 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_00CED32F-if00-port0 -P RIG -s 38400 T 0 t
