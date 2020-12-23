# WSPR CUI code

Taken from WSJT-X 2.3.0-rc2

## Tips

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

