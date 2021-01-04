#!/bin/bash
# WSPR Audio Recorder Script by DJ0ABR
# Modified by Kenji Rikitake, JJ1BDX
# record WSPR signal at every even minute
# called by cronjob

BASE=/home/kenji/wspr-trx
BASEWAV=${BASE}/wav
AUDIOINPUT="ALSA_AUDIO_INPUT_DEVICE"

# number of files in wav folder
cd ${BASEWAV}
file_num=$(ls -1 --file-type | grep -v '/$' | wc -l)
cd ${BASE}
if [ "$file_num" -le "1" ] ; then
    DT=$(date -u +"%y%m%d_%H%M")
    # Record 112 seconds, signed-integer 16bit PCM, 12000Hz
    arecord -D ${AUDIOINPUT} \
        -d 112 -f S16_LE -r 12000 -c 1 -t wav \
	${BASEWAV}/wspr_${DT}.wav
    # Spawn decoding process in the background
    ./decode.sh ${DT} &
fi
