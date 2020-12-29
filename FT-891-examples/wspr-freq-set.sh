#!/bin/sh
# FT-891: DATA-U, 350Hz, 7.0386MHz, Pwr 5W, IPO (PREAMP 0), AGC Auto (see agc_level_e in rig.h)
rigctl -m 1036 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_00CED32F-if00-port0 -P RIG -s 38400 - << .EOF.
M PKTUSB 350
F 7038600
L RFPOWER 0.05
L PREAMP 0
L AGC 6
.EOF.
