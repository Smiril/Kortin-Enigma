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

static void *coin(int fex)
{
    bool isDuplicate = false;
    //---------------------------------------------------------------------
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int max_index = (strlen(charset) - 1);
    //---------------------------------------------------------------------
    char picked[fex];
    memset( picked, 0, fex );
    char number[1];
    int j = 0,i = 0;

    for (i = 0; i < fex; i++) {
        do {
            int target = rand() % max_index; // Generate
            strncpy((char *)number,&charset[target],strlen(number)); // Pick
            // Check for duplicates
            for (j = 0; j < fex; j++) {
                if (number == &picked[j]) {
                    isDuplicate = true;
                    break; // Duplicate detected
                } // end if
                else {
                    //memmove((void *)&charset[max_index], &charset[max_index], max_index - strlen(number));
                    //max_index--;
                    strncpy((char *)&picked[j],number,strlen(number)); // picked
                    //isDuplicate = false;
                    // break; // No Duplicate detected
                } // end else
            } // end for
        } // end do
        while (isDuplicate); // equivalent to while(isDuplicate == true)
    } // end for
    printf("%s\n",picked);
   
    return 0;
}

int main(void)
{
    int i = 0;
    time_t start, stop;
    start = time(NULL);
    for (i = 0; i < 6; i++) {
        coin(26);
    } // end for
    coin(5);
    stop = time(NULL);
    printf("Time elapsed : %ld seconds\n",(stop - start));

    return 0;
}
