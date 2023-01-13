/*****************************************************************************
*
* This source code is distributed under the MIT license, COPYING.
*
* Author: Smiril <sonar@gmx.com>
* Date...: 11-12.01.2023
* Info......: See docs/credits.txt
* License.....: MIT
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <iconv.h>

#define CA_PRIVATE_IMPLEMENTATION


static char coin(int fex)
{
    bool isDuplicate = false;
    //---------------------------------------------------------------------
    const char *charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int max_index = (strlen(charset) - 1);
    //---------------------------------------------------------------------
    char picked[fex];
    char number[max_index];
    int j = 0,i = 0;

    for (i = 0; i <= fex; i++) {
        memset( picked, 0, fex );
        do {
            int target = rand() % max_index; // Generate
            strncpy((char *)number,&charset[target],strlen(number)); // Pick
            // Check for duplicates
            for (j = 0; j < 1; j++) {
                if (number == &picked[j]) {
                    isDuplicate = true;
                    break; // Duplicate detected
                } // end if
                else {
                    strncpy((char *)&picked[j],number,strlen(number)); // picked
                    printf("%c",picked[j]);
                    //return picked[j];
                    //break; // No Duplicate detected
                } // end else
            } // end for
        } // end do
        while (isDuplicate); // equivalent to while(isDuplicate == true)

    } // end for
    printf("\n");
   
    return 0;
}

int main(void)
{
    int i = 0;
    time_t start, stop;
    start = time(NULL);
    for (i = 0; i < 6; i++) {
        coin(25);
    } // end for
    coin(4);
    stop = time(NULL);
    printf("Time elapsed : %ld seconds\n",(stop - start));

    return 0;
}

