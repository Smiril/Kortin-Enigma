//
//  add.h
//  enigma
//
//  Created by Smiril on 20.07.22.
//  Copyright © 2022 Smiril. All rights reserved.
//

#ifndef add_h
#define add_h
#include "air.h"
#include "addx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <iconv.h>

char x[MSGLEN] = "A";
char s[MSGP] = "A";
char fff[MSGLEN] = "A";
char nerd[MSGX];
char framex[MSGY];
char ukw[MSGC];
char nox[MSGG];
int oldrank = IN_RANK;

/* Rotor wirings */
char rotor[MSGG][MSGC]={
    /* CHANGE THIS BLOCK 1-5+ref1-6+notch1-6 */
    /* Input "ABCDEFGHIJKLMNOPQRSTUVWXYZ" */
    /* 1: */ "WTOKASUYVRBXJHQCPZEFMDINLG",
    /* 2: */ "GJLPUBSWEMCTQVHXAOFZDRKYNI",
    /* 3: */ "JWFMHNBPUSDYTIXVZGRQLAOEKC",
    /* 4: */ "ESOVPZJAYQUIRHXLNFTGKDCMWB",
    /* 5: */ "HEJXQOTZBVFDASCILWPGYNMURK"};
char ref1[MSGC]="MOWJYPUXNDSRAIBFVLKZGQCHET";
char ref2[MSGC]="EJMZALYXVBWFCRQUONTSPIKHGD";
char ref3[MSGC]="YRUHQSLDPXNGOKMIEBFZCWVJAT";
char ref4[MSGC]="FVPJIAOYEDRZXWGCTKUQSBNMHL";
char ref5[MSGC]="ENKQAUYWJICOPBLMDXZVFTHRGS";
char ref6[MSGC]="RDOBJNTKVEHMLFCWZAXGYIPSUQ";
char refx[MSGC];
char notch1[MSGG]="YMDRH";
char notch2[MSGG]="HRDMY";
char notch3[MSGG]="YMDRH";
char notch4[MSGG]="HRDMY";
char notch5[MSGG]="YMDRH";
char notch6[MSGG]="HRDMY";
char notchx[MSGG];
/* Encryption parameters follow */

#endif /* add_h */
