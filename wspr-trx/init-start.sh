#!/bin/sh
BASE=/home/kenji/wspr-trx
BASEWAV=${BASE}/wav

rm ${BASEWAV}/*.wav
python3 ${BASE}/wspr-trx-event-loop.py
