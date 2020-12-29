#!/bin/sh
#
# Running this script will send a prebuilt WAV file as a WSPR beacon
# Set rig to PKTUSB mode
# Note: use Hamlib 4.1 (git master branch) or later
#
# Turn on PTT: enable TX
rigctl -m 1036 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_00CED32F-if00-port0 -P RIG -s 38400 T 1 t
# Send sound
aplay -D hw:3,0 ./jj1bdx-wspr-1536hz.wav
# Turn off PTT: disable TX
rigctl -m 1036 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_00CED32F-if00-port0 -P RIG -s 38400 T 0 t
