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

#define CA_PRIVATE_IMPLEMENTATION

static const char *coin(int fex) {

    bool isDuplicate = false;

    //---------------------------------------------------------------------

    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int max_index = (sizeof(charset) - 1);

    //---------------------------------------------------------------------

    const char *picked = NULL;

    const char *number = NULL;

    int j = 0,i = 0;

    for (i = 0; i < fex; i++)

    {

        do
        {

            int target = rand() % max_index; // Generate

            strncpy((char *)number,&charset[target],1); // Pick

            // Check for duplicates

            for (j = 0; j < fex; j++)
            {

                if (number == &picked[j])
                {

                    isDuplicate = true;

                    break; // Duplicate detected

                }
                else {

                    memmove((void *)&charset[target], &charset[target + 1], max_index - target);

                    max_index--;

                    isDuplicate = false;

                    break; // No Duplicate detected

                }

            } // end for

        } // end do
        while (isDuplicate); // equivalent to while(isDuplicate == true)

        if (!isDuplicate) { // equivalent to if(isDuplicate == false)

            strncpy((char *)&picked[j],number,1); // picked

        }

    } // end for

    return picked;

}

int main(void) {

    time_t start, stop;

    start = time(NULL);

    for (int i = 0; i < 6; i++) {

        printf("%s\n", coin(26));

    }

    printf("%s\n",coin(5));

    stop = time(NULL);

    printf("Time elapsed : %ld seconds\n",(stop - start));

    return 0;

}
