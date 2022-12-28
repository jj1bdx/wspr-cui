# Script for the WSJT-X/JTDX WSPR decoder, by DJ0ABR
# Modified by Kenji Rikitake, JJ1BDX

# Use the WSJT-X/JTDX decoder to get the spots out of the wav sound file
# The debug information are output to stdout
# The file wspr_spots.txt contains the current spots in the format for wsprnet.org
# wsprdsum.out is used as a temporary storage if the upload fails

BASE=/home/kenji/wspr-trx
BASEWAV=${BASE}/wav

# Obtain VFO frequency from the TRX every time
FREQHZ=`rigctl -m 1036 -r /dev/serial/by-id/usb-Silicon_Labs_CP2105_Dual_USB_to_UART_Bridge_Controller_00CED32F-if00-port0 -P RIG -s 38400 f`
FREQMHZ=`calc -p ${FREQHZ}/1000000`
echo "FREQMHZ=${FREQMHZ}"

MYCALL="YOUR_CALLSIGN"
MYGRID="YOUR_GRID_LOCATOR"
MYVERSION="wspr-cui"

cd ${BASE}

wsprd -f ${FREQMHZ} ${BASEWAV}/wspr_${1}.wav

# the wav file is processed, delete it (it should be in a ram disk folder if an SD card is used !)
rm ${BASEWAV}/wspr_${1}.wav

# check if spots are available
FILESIZE=$(stat -c%s "${BASE}/wspr_spots.txt")
if [ $FILESIZE -ne 0 ] ; then

    # add the spots to a temporary file used for uploading to wsprnet.org
    echo add to wsprdsum.out
    cat ${BASE}/wspr_spots.txt >> ${BASE}/wsprdsum.out

    # upload the spots
    echo upload by curl
    curl -s -S \
        -F allmept=@${BASE}/wsprdsum.out \
	-F call=${MYCALL} -F grid=${MYGRID} \
	-F version=${MYVERSION} \
	--connect-timeout 5 --max-time 8 \
	http://wsprnet.org/meptspots.php > /dev/null;
    RESULT=$?

    # check if curl uploaded the data successfully
    # delete only if uploaded
    if [ $RESULT -eq 0 ] ; then
        # data uploaded, delete them
        echo Upload OK, deleting wsprdsum.out
        rm ${BASE}/wsprdsum.out
    fi
    echo curl result: $RESULT , done.

fi
