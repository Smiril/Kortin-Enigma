//
//  add.h
//  enigma
//
//  Created by Smiril on 20.07.22.
//  Copyright © 2022 Smiril. All rights reserved.
//

#ifndef add_h
#define add_h

#include "addx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <iconv.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

pthread_mutex_t lock;

char *x = "A";
char *s = "A";
char *fff = "A";
char nerd[MSGX];
char framex[MSGY];
char flames[MSGY];
char *ukw;
char *nox;
int oldrank = IN_RANK;


/* Encryption parameters follow */
typedef struct main_ctx
{
        /* Rotor wirings */
        char rotor[MSGG][MSGC];
        char ref1[MSGXX][MSGC];
        char notch1[MSGXX][MSGG];
        char order[MSGG];/*={ 1, 3, 5, 7, 9 };*/
        char rings[MSGG];/*={ 'A','A','A','A','A'};*/
        char pos[MSGG];/*={ 'A','A','A','A','A'};*/
        char cyph[MSGLEN];
        char crib[MSGLEN];
        char plug[MSGP];/*=\"ASKINGLOPE\";*/
} main_ctx_t;

#endif /* add_h */
