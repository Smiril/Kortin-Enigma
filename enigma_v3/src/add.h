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
char flames[MSGY];
char ukw[MSGC];
char nox[MSGG];
int oldrank = IN_RANK;

/* Rotor wirings */
char rotor[MSGG][MSGC];
char ref1[MSGC];
char notch1[MSGG];
/* Encryption parameters follow */

#endif /* add_h */
