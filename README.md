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
