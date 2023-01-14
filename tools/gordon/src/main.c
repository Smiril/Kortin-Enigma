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

static char coin(int fex,int war)
{
    bool isDuplicate = false;
    //---------------------------------------------------------------------
    char charset[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int max_index = (strlen(charset) - 1);
    //int max_index = (sizeof(charset) - 1);
    //---------------------------------------------------------------------
    char picked[fex];
    char newabc[1];
    int j = 0,i = 0;

    for (i = 0; i <= fex; i++) {
        memset( picked, 0, fex );
        do {
            newabc[0] = charset[random() % max_index];
            // Check for duplicates
            for (j = 0; j <= fex; j++) {
                if (&newabc[0] == &picked[j]) {
                    isDuplicate = true;
                    break; // Duplicate detected
                } // end if
                else if(&newabc[0] != &picked[j]){
                    if (war == 0) {
                        strncpy((char *)&picked[j],&newabc[0],strlen(&newabc[0])); // picked
                        printf("%c",picked[j]);
                        memmove((void *)&charset[max_index],(void *)&charset[(max_index - atoi(newabc))],max_index - atoi(newabc));
                    }
                    isDuplicate = false;
                    break;
                } // end else if
                else {
                    printf("Error\n");
                    return -1;
                } // end else
            } // end for
        } // end do
        while (isDuplicate && j == fex); // equivalent to while(isDuplicate == false)
        
        if (war == 1) {
            strncpy((char *)&picked[j],&newabc[0],strlen(&newabc[0])); // picked
            printf("%c",picked[j]);
            memmove((void *)&charset[max_index],&charset[(max_index - atoi(newabc))],max_index - atoi(newabc));
        }
    } // end for
    printf("\n");

    return 0;
}

int main(int argc,char **argv)
{
    
    if(argc < 2 || argc > 2){ /*main case*/
          printf("\x1b[32m");
          printf("\nOption usage: %s <num>\n",argv[0]);
          printf("\x1b[0m");
          return -1;
      }
    
    int i = 0;
    time_t start, stop;
    start = time(NULL);
    for (i = 0; i < atoi(argv[1]); i++) {
        printf("\x1b[35m");
        coin(4,1);
        printf("\x1b[0m");
    } // end for
    for (i = 0; i < atoi(argv[1]); i++) {
        printf("\x1b[32m");
        coin(4,0);
        printf("\x1b[0m");
    } // end for
    stop = time(NULL);
    printf("Time elapsed : %ld seconds\n",(stop - start));

    return 0;
}
