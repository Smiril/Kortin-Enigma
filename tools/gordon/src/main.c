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
    int j = 0,i = 0,k = 0;

    for (i = 0; i <= fex; i++) {
        memset( picked, 0, fex );
        do {
            for (k = 0 ; k <= 1; k++) {
                newabc[k] = charset[rand() % max_index];
            } // end for
            // Check for duplicates
            for (j = 0; j <= fex; j++) {
                if (&newabc[k] == &picked[j]) {
                        isDuplicate = true;
                        break; // Duplicate detected
                } // end if
                else {
                    memmove((void *)&charset[max_index],&charset[max_index],max_index - strlen(&newabc[k]));
                    isDuplicate = false;
                } // end else
            } // end for
        } // end do
        while (isDuplicate); // equivalent to while(isDuplicate == true)
        
        strncpy((char *)&picked[j],&newabc[k],strlen(&newabc[k])); // picked
        printf("%c",picked[j]);
        
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
