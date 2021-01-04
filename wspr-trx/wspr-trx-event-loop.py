#!/usr/bin/env python3

# WSPR TX/RX event loop by Kenji Rikitake, JJ1BDX

from apscheduler.schedulers.blocking import BlockingScheduler
import subprocess
import random
import serial

# probability of transmission in percent times 2
# (40 for 20% probability)
txpercent = 40
# PTT control port (FT-891 with SCU-17 here, assert RTS)
pttport = "SCU_17_PTT_PORT(if01)"
# Remembering transmission state of txrxjob()
lasttx = False

def txrxjob():
    # Required: otherwise lasttx is treated as a local variable
    global lasttx
    print("TX/RX event loop:", end=" ", flush=True)
    subprocess.run(["date", "--rfc-3339=ns"])
    # Randomize
    x = random.randrange(0, 99)
    # Do not transmit continuously in multiple time slots
    if ((x < txpercent) and (lasttx == False)):
      # Turn PTT On
      s = serial.serial_for_url(pttport)
      s.rts=False 
      print("Enable transmission", flush=True)
      s.rts=True
      print("Start transmission", flush=True)
      # Play back audio
      subprocess.run(["aplay", "-q", "-D", "AUDIO_OUTPUT_DEVICE", "full path of the transmitted WAV file"])
      # Turn PTT Off
      s.rts=False 
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
