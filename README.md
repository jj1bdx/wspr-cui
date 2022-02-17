# WSPR CUI code

* Based on wsprd and wsprcode source taken from WSJT-X 2.3.0-rc2
* wsprsimwav derived from wsprsim
* Code of CUI interface for WSPR TX/RX is at `wspr-trx/`

## Requirements

* Linux or macOS
* GNU Fortran for compiling WSJT-X code
  - See the latest WSJT-X source code from [WSJT-X Web Page](https://physics.princeton.edu/pulsar/k1jt/wsjtx.html) for how to build the code in details
* [Advanced Python Scheduler](https://apscheduler.readthedocs.io/en/3.x/)

## How to build

```
cd wsprd
make clean && make
```

## Code formatting

* [clang-format](https://clang.llvm.org/docs/ClangFormatStyleOptions.html) for C code
* [fprettify](https://github.com/pseewald/fprettify) for Fortran code

Use `make format` to format the code.

## Tips

Note: tips for only WSPR-2 minute protocol are described unless otherwise noted

### For generating wav file

* Use wsprsimwav for generating a 48kHz mono WAV file, directly usable for transmission over USB transmitter

### For reading wav file by wsprd

* WAV header (first 22 bytes) are ignored
* Format: fixed to S16\_LE, 12000Hz, monaural (1 channel)
* Length: 114 seconds (see `readwavfile()` in wsprd.c) 

### .c2 file format by wsprd

* Length per file: 360026 bytes
* Header: 26 bytes
  -  Filename: first 14 bytes as char
  - WSPR type: next 4 bytes as int (2 for WSPR-2, 15 for WSPR-15)
  - Frequency: next 8 bytes as double in MHz
* Interleaved IQ signal
  - Strictly 45000 frames for 120 seconds of 375 frames/second
  - each frame has two float numbers (4 x 2 = 8 bytes / frame)
  - frame data: I, and -Q (negated Q)

### For generating .c2 file

* wsprsim generates no noise if SNR is 40 or more
* wsprsim generates reverse shift signal of `-f` option when processed with the following csdr script

### Make .c2 file audible

* Note: this sound output *does not decode* on WSJT-X
* Do not use this output for sending WSPR on the air

```
# upsample from 375Hz to 12000Hz sampling rate (with +-120Hz LPF)
# then shifting up 1500Hz (for make the signal audible)
# Output level: -6dB rms 
tail -c +27 201223_0000.c2 | 
    csdr plain_interpolate_cc 32 | 
    csdr bandpass_fir_fft_cc -0.01 0.01 0.001 | 
    csdr gain_ff 16 | csdr shift_math_cc 0.125 | 
    play --buffer=1024 -t raw -e floating-point -b32 -r 12000 -c 2 -q -
```

### WSPR-2 timing and frequency

* The channel symbol output corresponds to lowest to highest frequencies, from 0 to 3, with 1.46484375 (= 12000/8192) Hz frequency shift, respectively.
* The channel spacing diagram from the center frequency:
  - Symbol 0: -2.197265625 Hz ( = -18000/8192 Hz)
  - Symbol 1: -0.732421875 Hz ( = -6000/8192 Hz)
  - Symbol 2: +0.732421875 Hz ( = +6000/8192 Hz)
  - Symbol 3: +2.197265625 Hz ( = +18000/8192 Hz)
* Transmission rate: 1.46484375 baud = 0.682666667 second/symbol = 8192/12000 second
* Each transmission contains 162 symbols for 110.592 seconds
* Each transmission starts +1 second after even UTC minute (e.g., 00:01, 02:01)

## LICENSE

GPLv3
