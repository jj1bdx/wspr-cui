#!/usr/bin/env python3

# WSPR TX/RX event loop by Kenji Rikitake, JJ1BDX

from apscheduler.schedulers.blocking import BlockingScheduler
import subprocess
import random
import serial
import signal
import sys
import time

# probability of transmission in percent
# (50% maximum)
txpercent = 20
# Remembering transmission state of txrxjob()
lasttx = False

# PTT control port (SCU-17 connected to FT-891 here, assert RTS for PTT)
pttport = "SCU_17_PTT_PORT(if01)"
# Initialize PTT port as a global PySerial object
pttobject = serial.serial_for_url(pttport, rtscts = False, dsrdtr = False, do_not_open=True)

def pttobject_set_rts(value):
    # You cannot reopen a port
    # The port must be closed before it is reopened
    if pttobject.is_open:
      pttobject.close()
    # Set RTS value
    pttobject.rts = value
    # Change to the control line state of a serial port
    # requires open()ing the port
    pttobject.open()

def signal_handler(signal, frame):
    print("Disable transmission", flush=True)
    pttobject_set_rts(False)
    print('Terminated by CTRL/C')
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

def txrxjob():
    # Required: otherwise lasttx is treated as a local variable
    global lasttx
    print("TX/RX event loop:", end=" ", flush=True)
    subprocess.run(["date", "--rfc-3339=ns"])
    # Randomize
    x = random.randrange(0, 99)
    # Do not transmit continuously in multiple time slots
    if ((x < txpercent) and (lasttx == False)):
      # Turn PTT On (but turn off first)
      pttobject_set_rts(False)
      print("Enable transmission", flush=True)
      pttobject_set_rts(True)
      print("Start transmission", flush=True)
      # Play back audio
      subprocess.run(["aplay", "-q", "-D", "AUDIO_OUTPUT_DEVICE", "full path of the transmitted WAV file"])
      # Turn PTT Off
      pttobject_set_rts(False)
      print("Complete transmission", flush=True)
      print("Transmission completed at:", end=" ", flush=True)
      subprocess.run(["date", "--rfc-3339=ns"])
      lasttx = True
    else:
      # Invoke receiving code
      print("Start reception", flush=True)
      subprocess.run(["sh", "full path of the record.sh script here"])
      print("Complete reception", flush=True)
      print("Reception completed at:", end=" ", flush=True)
      subprocess.run(["date", "--rfc-3339=ns"])
      lasttx = False

# Invoking scheduler
random.seed()
scheduler = BlockingScheduler()
scheduler.add_job(txrxjob, 'cron', minute='*/2', second=0)
scheduler.start()

# End of code
