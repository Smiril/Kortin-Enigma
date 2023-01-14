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
    char charset[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //int max_index = (strlen(charset));
    int max_index = (sizeof(charset) - 1);
    //---------------------------------------------------------------------
    ///int fox = malloc(fex);
    char picked[fex];
    //char number[1];
    char newabc[1];
    int j = 0,i = 0;
    
    for (i = 0; i <= fex; i++) {
        memset( picked, 0, fex );
        do {
            newabc[0] = charset[rand() % max_index];
            // Check for duplicates
            for (j = 0; j <= fex; j++) {
                if (&newabc[0] == &picked[j]) {
                    isDuplicate = true;
                    break; // Duplicate detected
                } // end if
                else {
                    strncpy((char *)&picked[j],&newabc[0],strlen(&newabc[0])); // picked
                    printf("%c",picked[j]);
                    memmove((void *)&charset[max_index],&charset[max_index],max_index - 1);
                    break;
                    //isDuplicate = false;
                } // end else
            } // end for
        } // end do
        while (isDuplicate); // equivalent to while(isDuplicate == true)
        
        //strncpy((char *)&picked[j],&newabc[1],strlen(&newabc[1])); // picked
        //printf("%c",picked[j]);
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
