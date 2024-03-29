//  Generating 48kHz WAV file for a WSPR-encoded signal
//
//  File name: wsprsimwav.c
//  Modified from: wsprsim.c
//

#include <getopt.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fano.h"
#include "wsprd_utils.h"
#include "wsprsim_utils.h"

int printdata = 0;

void usage() {
  printf("wsprsimwav: generate 48kHz S16_LE mono WAV file\n");
  printf("\n");
  printf("Usage: wsprsimwav [options] message\n");
  printf("       message format:   \"K1ABC FN42 33\"\n");
  printf("                         \"PJ4/K1ABC 33\"\n");
  printf("                         \"<PJ4/K1ABC> FK52UD 33\"\n");
  printf("Options:\n");
  printf("       -a x (x: output level in dbFS )\n");
  printf("       -c   (print channel symbols)\n");
  printf("       -d   (print packed data with zero tail - 11 bytes)\n");
  printf("       -f x (-100 Hz < f < 100 Hz, center: 1500 Hz)\n");
  printf("       -o filename (write a wav file with this name)\n");
  printf("\n");
  printf(" e.g. ./wsprsimwav -a -6 -cd -o k1abc.wav \"K1ABC FN42 33\"\n");
}

#define SRATE (48000.0)   // sampling rate = 48000 Hz
#define TRATE (32768)     // symbol length by samples
#define NSYM (162)        // number of symbols for each transmission
#define SYMSIZE (5376000) // 112 seconds for 48000 samples/second
#define FOFFSET (1500.0)  // center frequency for audio signal

// slope rate for 20 milliseconds by samples
#define SLOPERATE (0.02 * SRATE)
// signal length in samples
#define SIGNALLENGTH (TRATE * NSYM)

// Calculate Raised Cosine slope
double raised_cosine(double x) {
  if ((x <= -1.0) || (x >= 1.0)) {
    return 0;
  }
  return (0.5 * (1.0 + cos(M_PI * x)));
}

// Audio volume envelope generator
// to fade in/out with raised-cosine envelopes
// (see https://github.com/jj1bdx/wspr-cui/issues/5 for the details)
double volume_sloper(int pos) {
  if ((0 <= pos) && (pos < SLOPERATE)) {
    return raised_cosine(1.0 - ((double)pos / SLOPERATE));
  }
  if (((SIGNALLENGTH - SLOPERATE) <= pos) && (pos < SIGNALLENGTH)) {
    int newpos = pos - (SIGNALLENGTH - SLOPERATE);
    return raised_cosine((double)newpos / SLOPERATE);
  }
  if ((pos < 0) || (pos >= SIGNALLENGTH)) {
    return 0.0;
  }
  return 1.0;
}

void add_signal_vector(double f0, double amp, unsigned char *symbols,
                       double sig[]) {
  int i, j, ii, idelay;
  double phi = 0.0, twopidt, df, dphi;

  twopidt = 8.0 * atan(1.0) / SRATE; // 2 * PI / SRATE
  df = SRATE / (double)TRATE;
  idelay = SRATE; // 1.0 second delay

  for (i = 0; i < NSYM; i++) {
    dphi = twopidt * (FOFFSET + f0 + (((double)symbols[i] - 1.5) * df));
    for (j = 0; j < TRATE; j++) {
      ii = idelay + (TRATE * i) + j;
      sig[ii] = sin(phi) * amp * volume_sloper(ii - idelay);
      phi = phi + dphi;
    }
  }
}

char *tobinary(int x) {
  static char b[33];
  b[0] = '\0';

  long unsigned int z;
  for (z = 0x80000000; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

// See
// https://stackoverflow.com/questions/23030980/creating-a-stereo-wav-file-using-c
// https://docs.fileformat.com/audio/wav/

typedef struct wavfile_header_s {
  char ChunkID[4];   /*  4   */
  int32_t ChunkSize; /*  4   */
  char Format[4];    /*  4   */

  char Subchunk1ID[4];   /*  4   */
  int32_t Subchunk1Size; /*  4   */
  int16_t AudioFormat;   /*  2   */
  int16_t NumChannels;   /*  2   */
  int32_t SampleRate;    /*  4   */
  int32_t ByteRate;      /*  4   */
  int16_t BlockAlign;    /*  2   */
  int16_t BitsPerSample; /*  2   */

  char Subchunk2ID[4];
  int32_t Subchunk2Size;
} wavfile_header_t;

#define SUBCHUNK1SIZE (16)
#define AUDIO_FORMAT (1) /*For PCM*/

void writewavfile(char *wavfilename, double *sig) {
  wavfile_header_t wav_header;
  FILE *fp;
  int32_t subchunk2_size, chunk_size;
  int32_t val;
  double v, vlim;
  uint32_t o;
  uint8_t ol, ou;

  subchunk2_size = SYMSIZE * 2; // mono, 16bit signed-integer PCM
  chunk_size = 4 + (8 + SUBCHUNK1SIZE) + (8 + subchunk2_size);

  fp = fopen(wavfilename, "wb");
  if (fp == NULL) {
    fprintf(stderr, "Could not open wav file '%s'\n", wavfilename);
    exit(-1);
  }

  wav_header.ChunkID[0] = 'R';
  wav_header.ChunkID[1] = 'I';
  wav_header.ChunkID[2] = 'F';
  wav_header.ChunkID[3] = 'F';

  wav_header.ChunkSize = chunk_size;

  wav_header.Format[0] = 'W';
  wav_header.Format[1] = 'A';
  wav_header.Format[2] = 'V';
  wav_header.Format[3] = 'E';

  wav_header.Subchunk1ID[0] = 'f';
  wav_header.Subchunk1ID[1] = 'm';
  wav_header.Subchunk1ID[2] = 't';
  wav_header.Subchunk1ID[3] = ' ';

  wav_header.Subchunk1Size = SUBCHUNK1SIZE;
  wav_header.AudioFormat = 1; // for PCM
  wav_header.NumChannels = 1; // monaural
  wav_header.SampleRate = SRATE;
  wav_header.ByteRate = SRATE * 2; // 16bit signed-integer
  wav_header.BlockAlign = 2;       // mono, 16bit signed-integer
  wav_header.BitsPerSample = 16;   // 16bit signed-integer

  wav_header.Subchunk2ID[0] = 'd';
  wav_header.Subchunk2ID[1] = 'a';
  wav_header.Subchunk2ID[2] = 't';
  wav_header.Subchunk2ID[3] = 'a';
  wav_header.Subchunk2Size = subchunk2_size;

  (void)fwrite(&wav_header, sizeof(wavfile_header_t), 1, fp);

  for (int32_t i = 0; i < SYMSIZE; i++) {
    v = sig[i];
    vlim = fmax(-1.0, fmin(1.0, v)); // clipping
    val = lrint(vlim * 32767);
    o = val;
    ol = o & 0xff;
    ou = (o >> 8) & 0xff;
    fwrite(&ol, sizeof(uint8_t), 1, fp);
    fwrite(&ou, sizeof(uint8_t), 1, fp);
  }

  fclose(fp);
}

int main(int argc, char *argv[]) {
  extern char *optarg;
  extern int optind;
  int i, c, printchannel = 0, writewav = 1;
  double f0 = 0.0, adb = 0.0;
  double *sig;
  char *message, *wavfilename, *hashtab, *loctab;

  wavfilename = malloc(sizeof(char) * 256);
  hashtab = malloc(sizeof(char) * 32768 * 13);
  loctab = malloc(sizeof(char) * 32768 * 5);
  memset(hashtab, 0, sizeof(char) * 32768 * 13);
  memset(hashtab, 0, sizeof(char) * 32768 * 5);

  // message length is 22 characters
  message = malloc(sizeof(char) * 23);

  strcpy(wavfilename, "wsprsimwav.wav");

  while ((c = getopt(argc, argv, "a:cdf:o:")) != -1) {
    switch (c) {
    case 'a':
      adb = atof(optarg);
      break;
    case 'c':
      printchannel = 1;
      break;
    case 'd':
      printdata = 1;
      break;
    case 'f':
      f0 = atof(optarg);
      break;
    case 'o':
      wavfilename = optarg;
      writewav = 1;
      break;
    }
  }

  if (optind + 1 > argc) {
    usage();
    return 0;
  } else {
    message = argv[optind];
  }

  unsigned char channel_symbols[NSYM];
  get_wspr_channel_symbols(message, hashtab, loctab, channel_symbols);

  if (printchannel) {
    printf("Channel symbols:\n");
    for (i = 0; i < 162; i++) {
      printf("%d ", channel_symbols[i]);
    }
    printf("\n");
  }

  if (writewav) {
    // add signal
    sig = malloc(sizeof(double) * SYMSIZE);
    memset(sig, 0, sizeof(double) * SYMSIZE);

    add_signal_vector(f0, pow(10.0, adb / 20.0), channel_symbols, sig);

    printf("Writing %s\n", wavfilename);
    writewavfile(wavfilename, sig);
  }
  return 1;
}
