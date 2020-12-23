#ifndef WSPRSIM_UTILS_H
#define WSPRSIM_UTILS_H

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern int printdata;

int get_wspr_channel_symbols(char *message, char *hashtab, char *loctab,
                             unsigned char *symbols);

#endif
