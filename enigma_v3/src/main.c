/*****************************************************************************
*
* This source code is distributed under the MIT license, COPYING.
*
* Author: Smiril <sonar@gmx.com>
* Based in the code of:
*  capi_x <capi_x@haibane.org>
*  Harald Schmidl <hjschmidl@acm.org>
*  Fauzan Mirza's <fauzan@dcs.rhbnc.ac.uk>
*
* Info......: See docs/credits.txt
* License.....: MIT
******************************************************************************/
#define CA_PRIVATE_IMPLEMENTATION

#include "add.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "types.h"
#include "memory.h"
#include "user_options.h"
#include "hashcat.h"
#include "terminal.h"
#include "thread.h"
#include "status.h"
#include "shared.h"
#include "event.h"

//---------------------------------------------------------------------
const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int max_index = (sizeof(charset) - 1);
//---------------------------------------------------------------------

void configmain(char *hugh) {
    // open file
    FILE *fp = fopen(hugh, "r");
    size_t len = 27;
    // need malloc memory for line, if not, segmentation fault error will occurred.
    char *line = malloc(sizeof(char) * len);
    // check if file exist (and you can open it) or not
    if (fp == NULL) {
        printf("can open file %s!\n",hugh);
        return;
    }
    while(fgets(line, len, fp) != NULL) {
        for (int i = 0; i < 6; i++){
            snprintf(rotor[i][MSGC],MSGC,"%s", line);
            }
            snprintf(ref1[MSGC],MSGC,"%s",line);
            snprintf(notch1[MSGG],MSGG,"%s",line);
    }
    free(line);
}

//---------------------------------------------------------------------
static const char *coinn(void) {
    bool isDuplicate = false;
    const char picked[MSGG];
    const char *number = NULL;
    int j = 0,i = 0;
    
    for (i = 0; i < 27; i++)
    {
        do
        {
            int target = rand() % max_index;
            strcpy(number,&charset[target]); // Generate the number
            memmove(&charset[target], &charset[target + 1], max_index - target);
            max_index--;
            // Check for duplicates
            for (j = 0; j < 5; j++)
            {
                if (number == picked[j])
                {
                    isDuplicate = true;
                    break; // Duplicate detected
                }
            }
        }
        while (isDuplicate); // equivalent to while(isDuplicate == true)
        
        strcpy(picked[j],number);
    }
    return picked;
}

static const char *coinr(void) {
    bool isDuplicate = false;
    const char picked[MSGC];
    const char *number = NULL;
    int j = 0,i = 0;
    
    for (i = 0; i < 27; i++)
    {
        do
        {
            int target = rand() % max_index;
            strcpy(number,&charset[target]); // Generate the number
            memmove(&charset[target], &charset[target + 1], max_index - target);
            max_index--;
            // Check for duplicates
            for (j = 0; j < 27; j++)
            {
                if (number == picked[j])
                {
                    isDuplicate = true;
                    break; // Duplicate detected
                }
            }
        }
        while (isDuplicate); // equivalent to while(isDuplicate == true)
        
        strcpy(picked[j],number);
    }
    return picked;
}


int getRank(char *cyph) {

    char str[40];
    int  rank = 0;

    FILE * dict = fopen(framex, "r");
        if(!dict) return -1; /* cant open file */
    
    while(fgets(str,sizeof(str),dict) != NULL) {
        str[strlen(str)-1] = '\0';
        if(strstr(cyph, str) != NULL) rank++;
    }

    fclose(dict);
    return(rank);
}

/*take a char and return its encoded version according to the
  encryption params, update params, i.e. advance wheels
  this part uses Fauzan Mirza's code*/
char scramble(char c, main_ctx_t *main_ctx)
{
    unsigned long int flag = 0;
    int j,i;
        c=toupper(c);
        if (!isalpha(c))
            return -1;
        if(isspace(c)) {
            c = ' ';
        }

    for(unsigned long int f = 0;f <= strlen(&c);f++) {
        /* Step up first rotor */
        main_ctx->pos[0]++;
        if (main_ctx->pos[0]>'Z')
            main_ctx->pos[0] -= 26;
        /*  Step up second rotor if first rotor reached notch */
        if (main_ctx->pos[0]==nox[main_ctx->order[0]-1])
        {
            main_ctx->pos[1]++;
            if (main_ctx->pos[1]>'Z')
                main_ctx->pos[1] -= 26;
            if (main_ctx->pos[0]==nox[main_ctx->order[1]-1])
            {
                main_ctx->pos[1]++;
                if (main_ctx->pos[1]>'Z')
                    main_ctx->pos[1] -= 26;
                if (main_ctx->pos[0]==nox[main_ctx->order[2]-1])
                {
                    main_ctx->pos[1]++;
                    if (main_ctx->pos[1]>'Z')
                        main_ctx->pos[1] -= 26;
                    if (main_ctx->pos[0]==nox[main_ctx->order[3]-1])
                    {
                        main_ctx->pos[1]++;
                        if (main_ctx->pos[1]>'Z')
                            main_ctx->pos[1] -= 26;
                        if (main_ctx->pos[0]==nox[main_ctx->order[4]-1])
                        {
                            main_ctx->pos[1]++;
                            if (main_ctx->pos[1]>'Z')
                                main_ctx->pos[1] -= 26;
                            flag=f;
            /* Set flag if second rotor reached notch */
                            if (main_ctx->pos[1]==nox[main_ctx->order[0]-1])
                            {
                                main_ctx->pos[2]++;
                                if (main_ctx->pos[2]>'Z')
                                    main_ctx->pos[2] -= 26;
                                if (main_ctx->pos[1]==nox[main_ctx->order[1]-1])
                                {
                                    main_ctx->pos[2]++;
                                    if (main_ctx->pos[2]>'Z')
                                        main_ctx->pos[2] -= 26;
                                    if (main_ctx->pos[1]==nox[main_ctx->order[2]-1])
                                    {
                                        main_ctx->pos[2]++;
                                        if (main_ctx->pos[2]>'Z')
                                            main_ctx->pos[2] -= 26;
                                        if (main_ctx->pos[1]==nox[main_ctx->order[3]-1])
                                        {
                                            main_ctx->pos[2]++;
                                            if (main_ctx->pos[2]>'Z')
                                                main_ctx->pos[2] -= 26;
                                            if (main_ctx->pos[1]==nox[main_ctx->order[4]-1])
                                            {
                                                main_ctx->pos[2]++;
                                                if (main_ctx->pos[2]>'Z')
                                                    main_ctx->pos[2] -= 26;
                                                flag=f;
                                                if (main_ctx->pos[2]==nox[main_ctx->order[0]-1])
                                                {
                                                    main_ctx->pos[3]++;
                                                    if (main_ctx->pos[3]>'Z')
                                                        main_ctx->pos[3] -= 26;
                                                    if (main_ctx->pos[2]==nox[main_ctx->order[1]-1])
                                                    {
                                                        main_ctx->pos[3]++;
                                                        if (main_ctx->pos[3]>'Z')
                                                            main_ctx->pos[3] -= 26;
                                                        if (main_ctx->pos[2]==nox[main_ctx->order[2]-1])
                                                        {
                                                            main_ctx->pos[3]++;
                                                            if (main_ctx->pos[3]>'Z')
                                                                main_ctx->pos[3] -= 26;
                                                            if (main_ctx->pos[2]==nox[main_ctx->order[3]-1])
                                                            {
                                                                main_ctx->pos[3]++;
                                                                if (main_ctx->pos[3]>'Z')
                                                                    main_ctx->pos[3] -= 26;
                                                                if (main_ctx->pos[2]==nox[main_ctx->order[4]-1])
                                                                {
                                                                    main_ctx->pos[3]++;
                                                                    if (main_ctx->pos[3]>'Z')
                                                                        main_ctx->pos[3] -= 26;
                                                                    flag=f;
                                                                    if (main_ctx->pos[3]==nox[main_ctx->order[0]-1])
                                                                    {
                                                                        main_ctx->pos[4]++;
                                                                        if (main_ctx->pos[4]>'Z')
                                                                            main_ctx->pos[4] -= 26;
                                                                        if (main_ctx->pos[3]==nox[main_ctx->order[1]-1])
                                                                        {
                                                                            main_ctx->pos[4]++;
                                                                            if (main_ctx->pos[4]>'Z')
                                                                                main_ctx->pos[4] -= 26;
                                                                            if (main_ctx->pos[3]==nox[main_ctx->order[2]-1])
                                                                            {
                                                                                main_ctx->pos[4]++;
                                                                                if (main_ctx->pos[4]>'Z')
                                                                                    main_ctx->pos[4] -= 26;
                                                                                if (main_ctx->pos[3]==nox[main_ctx->order[3]-1])
                                                                                {
                                                                                    main_ctx->pos[4]++;
                                                                                    if (main_ctx->pos[4]>'Z')
                                                                                        main_ctx->pos[4] -= 26;
                                                                                    if (main_ctx->pos[3]==nox[main_ctx->order[4]-1])
                                                                                    {
                                                                                        main_ctx->pos[4]++;
                                                                                        if (main_ctx->pos[4]>'Z')
                                                                                            main_ctx->pos[4] -= 26;
                                                                                        flag=f;
                                                                                        if (main_ctx->pos[4]==nox[main_ctx->order[0]-1])
                                                                                        {
                                                                                            if (main_ctx->pos[4]==nox[main_ctx->order[1]-1])
                                                                                            {
                                                                                                if (main_ctx->pos[4]==nox[main_ctx->order[2]-1])
                                                                                                {
                                                                                                    if (main_ctx->pos[4]==nox[main_ctx->order[3]-1])
                                                                                                    {
                                                                                                        if (main_ctx->pos[4]==nox[main_ctx->order[4]-1])
                                                                                                        {
                                                                                                            flag=f;
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    /* Check if rotor reached notch last time */
        if (flag)
        {
        /* Step up rotors */
            if (flag < 2)
            {
                f++;
                main_ctx->pos[0]++;
                if (main_ctx->pos[0]>'Z')
                    main_ctx->pos[0] -= 26;
            }
            if (flag > 1)
            {
                f++;
                main_ctx->pos[1]++;
                if (main_ctx->pos[1]>'Z')
                    main_ctx->pos[1] -= 26;
            }
            if (flag > 2)
            {
                f++;
                main_ctx->pos[2]++;
                if (main_ctx->pos[2]>'Z')
                    main_ctx->pos[2] -= 26;
            }
            if (flag > 3)
            {
                f++;
                main_ctx->pos[3]++;
                if (main_ctx->pos[3]>'Z')
                    main_ctx->pos[3] -= 26;
            }
            if (flag > 4)
            {
                f++;
                main_ctx->pos[4]++;
                if (main_ctx->pos[4]>'Z')
                    main_ctx->pos[4] -= 26;
                flag=0;
            }
        }
    }
    /*  Swap pairs of letters on the plugboard */
    for (unsigned long int k=0;main_ctx->plug[k]; k+=2)
    {
        if (c==main_ctx->plug[k])
            c=main_ctx->plug[k+1];
        else if (c==main_ctx->plug[k+1])
            c=main_ctx->plug[k];
    }

        /*  Rotors (forward) */
        for (i=0; i<5; i++)
        {
            c += main_ctx->pos[i]-'A';
            if (c>'Z')
                c -= 26;

            c -= main_ctx->rings[i]-'A';
            if (c<'A')
                c += 26;

            c=rotor[main_ctx->order[i]-1][c-'A'];

            c += main_ctx->rings[i]-'A';
            if (c>'Z')
                c -= 26;

            c -= main_ctx->pos[i]-'A';
            if (c<'A')
                c += 26;
        }

        /*  Reflecting rotor */

        c=ukw[c-'A'];
    
    /*  Rotors (reverse) */
        for (i=5; i; i--)
        {
            c += main_ctx->pos[i-1]-'A';
            if (c>'Z')
                c -= 26;

            c -= main_ctx->rings[i-1]-'A';
            if (c<'A')
                c += 26;

            for (j=0; j<26; j++)
                if (rotor[main_ctx->order[i-1]-1][j]==c)
                    break;
            c=j+'A';

            c += main_ctx->rings[i-1]-'A';
            if (c>'Z')
                c -= 26;

            c -= main_ctx->pos[i-1]-'A';
            if (c<'A')
                c += 26;
        }
        
    /*  Plugboard */
        for (unsigned long int k=0;main_ctx->plug[k]; k+=2)
        {
            if (c==main_ctx->plug[k])
                c=main_ctx->plug[k+1];
            else if (c==main_ctx->plug[k+1])
                c=main_ctx->plug[k];
        }

  return c;
}

/*take a string, return encoded string*/
char *enigma(char *in, main_ctx_t *main_ctx)
{
    unsigned long int j;
    for(j = 0; j < strlen(in); j++)
    x[j] = scramble(in[j], main_ctx);
    x[j] = '\0';
    return x;
}

/*read in a string, and pass it through enigma*/
void cypher(main_ctx_t main_ctx)
{
    char inx[MSGLEN], o[MSGLEN];
    char c;
    int i = 0;
  while((c = getchar()) != '\n')
  {
      if(isspace(c)) {
        c = '-';
      }
    inx[i] = toupper(c);
    i++;
  }
  inx[i] = '\0';
    strcpy(o, enigma(inx, &main_ctx));
    printf("%s\n%s\n", o, inx);
}

int rotate(main_ctx_t *main_ctx,int a, int b, int c, int d, int e, char *cyph, char *plug, int *ct)
{
    hashcat_ctx_t *hashcat_ctx = NULL;
    int rank = IN_RANK;
    
    main_ctx->order[0] = a;
    main_ctx->order[1] = b;
    main_ctx->order[2] = c;
    main_ctx->order[3] = d;
    main_ctx->order[4] = e;
    const time_t proc_start = time (NULL);
  //p.rings[0] = p.rings[1] = p.rings[2] = p.rings[3] = p.rings[4] = 'A';
  //p.pos[0] = p.pos[1] = p.pos[2] = p.pos[3] = p.pos[4] = 'A';
  strcpy(main_ctx->plug, plug);

  for(main_ctx->pos[0] = 'A'; main_ctx->pos[0] <= 'Z'; main_ctx->pos[0]++)
  {
    for(main_ctx->pos[1] = 'A'; main_ctx->pos[1] <= 'Z'; main_ctx->pos[1]++)
    {
        for(main_ctx->pos[2] = 'A'; main_ctx->pos[2] <= 'Z'; main_ctx->pos[2]++)
        {
            for(main_ctx->pos[3] = 'A'; main_ctx->pos[3] <= 'Z'; main_ctx->pos[3]++)
            {
                for(main_ctx->pos[4] = 'A'; main_ctx->pos[4] <= 'Z'; main_ctx->pos[4]++)
                {
                    for(main_ctx->rings[0] = 'A'; main_ctx->rings[0] <= 'Z'; main_ctx->rings[0]++)
                    {
                        for(main_ctx->rings[1] = 'A'; main_ctx->rings[1] <= 'Z'; main_ctx->rings[1]++)
                        {
                            for(main_ctx->rings[2] = 'A'; main_ctx->rings[2] <= 'Z'; main_ctx->rings[2]++)
                            {
                                for(main_ctx->rings[3] = 'A'; main_ctx->rings[3] <= 'Z'; main_ctx->rings[3]++)
                                {
                                    for(main_ctx->rings[4] = 'A'; main_ctx->rings[4] <= 'Z'; main_ctx->rings[4]++)
                                    {
                    
                                        //main_ctx_t *cp = main_ctx;
                                        unsigned long int i = 0;
                                        int fail = 0;
                                        
                                        /* Calculate all cypher */
                                        strlcpy(fff, enigma(cyph, main_ctx), sizeof(fff));
                                        rank = getRank(fff);

                                        while(strlen(fff) > i)
                                        {
                                            if(cyph[i] != scramble(fff[i], main_ctx)) fail++;
                                            
                                            /* if we are bruteforcing plug dont allow fails */
                                            if ( ( fail > 666 ) || (i == 0 && fail > 0 ) || (i == 1 && fail > 0) ) { fail = 666; break; }
                                            else
                                                i++;
                                        }
                                        
                                        if(strlen(fff) == i)
                                        {
                                        (ct)++;
                                            if ( fail <= 666 ) {
                                                /* XXX: Show result if is different to the last one */
                                                if( rank > oldrank ) {
                                                    printf("\x1B[33mWheels\x1B[39m \x1B[32m%d %d %d %d %d\x1B[39m \x1B[33mStart\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mRings\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mStecker\x1B[39m \"\x1B[32m%s\x1B[39m\" TEXT: %s Rank: %d\n",
                                                           main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4],
                                                           main_ctx->pos[0], main_ctx->pos[1], main_ctx->pos[2], main_ctx->pos[3], main_ctx->pos[4],
                                                           main_ctx->rings[0], main_ctx->rings[1], main_ctx->rings[2], main_ctx->rings[3], main_ctx->rings[4], main_ctx->plug, fff, rank);
                                                    oldrank = rank;
                                                }
                                                
                                            } else continue;
                                            
                                        if(strcmp(cyph, enigma(fff, main_ctx)) == 0)
                                        {
                                            printf("\x1B[33mWheels\x1B[39m \x1B[32m%d %d %d %d %d\x1B[39m \x1B[33mStart\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mRings\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mStecker\x1B[39m \"\x1B[32m%s\x1B[39m\"\nReflector: %s\nNOTCH: %s\n",
                                                   main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4],
                                                   main_ctx->pos[0], main_ctx->pos[1], main_ctx->pos[2], main_ctx->pos[3], main_ctx->pos[4],
                                                   main_ctx->rings[0], main_ctx->rings[1], main_ctx->rings[2], main_ctx->rings[3], main_ctx->rings[4], main_ctx->plug,ukw,nox);
                                            
                                            printf("%s decoded -> %s\n",cyph,enigma(cyph, main_ctx));
                                            const time_t proc_stop = time (NULL);
                                            printf("Time elapsed : begin %ld - end %ld \n",proc_start,proc_stop);
                                            hashcat_destroy (hashcat_ctx);
                                            return 1;
                                        } else continue;
                                    }
                                }
                            }
                        }
                    }
                }
            }
         }
      }
    }
  }
  return 0;
}

void test(main_ctx_t *main_ctx,int a, int b, int c, int d, int e, char *cyph, int *ct) {
   
    printf("... calculating\n");
    const time_t proc_start = time (NULL);
    char A, B, C, D, E, F, G, H, I, J;
    int i = 0,cs;
    
    strcpy(s, "");
    printf("... Checking wheels %d %d %d %d %d\n",  a, b, c, d, e);
    for(cs = 0; cs < 10; cs++)
    {
      if(cs > 0)
      {
        for(A = 'A'; A <= TO; A++)
        {
          for(B = A + 1; B <= TO; B++)
          {
              i++;
              s[0] = A;
              s[1] = B;
              s[2] = '\0';
              if(cs > 1)
              {
                  for(C = A + 1; C <= TO; C++)
                  {
                      if(A == C || B == C || C == 'Z')
                          continue;
                      for(D = C + 1; D <= TO; D++)
                      {
                          i++;
                          s[0] = A;
                          s[1] = B;
                          s[2] = C;
                          s[3] = D;
                          s[4] = '\0';
                          if(cs > 3)
                          {
                              if(A == D || B == D || C == D || D == 'Z')
                                  continue;
                              for(E = D + 1; E <= TO; E++)
                              {
                                  if(A == E || B == E || C == E || D == E || E == 'Z')
                                      continue;
                                  for(F = E + 1; F <= TO; F++)
                                  {
                                      i++;
                                      s[0] = A;
                                      s[1] = B;
                                      s[2] = C;
                                      s[3] = D;
                                      s[4] = E;
                                      s[5] = F;
                                      s[6] = '\0';
                                      if(cs > 5)
                                      {
                                          if(A == F || B == F || C == F || D == F || E == F || F == 'Z')
                                              continue;
                                          for(G = F + 1; G <= TO; G++)
                                          {
                                              if(A == G || B == G || C == G || D == G || E == G || F == G || G == 'Z')
                                                  continue;
                                              for(H = G + 1; H <= TO; H++)
                                              {
                                                  if(A == H || B == H || C == H || D == H || E == H || F == H || G == H || H == 'Z')
                                                      continue;
                                                  i++;
                                                  s[0] = A;
                                                  s[1] = B;
                                                  s[2] = C;
                                                  s[3] = D;
                                                  s[4] = E;
                                                  s[5] = F;
                                                  s[6] = G;
                                                  s[7] = H;
                                                  s[8] = '\0';
                                                  if(cs > 7)
                                                  {
                                                      for(I = H + 1; I <= TO; I++)
                                                      {
                                                          if(A == I || B == I || C == I || D == I || E == I || F == I || G == I || H == I || I == 'Z')
                                                              continue;
                                                          for(J = I + 1; J <= TO; J++)
                                                          {
                                                              if(A == J || B == J || C == J || D == J || E == J || F == J || G == J || H == J || I == J || J == 'Z')
                                                                  continue;
                                                              i++;
                                                              s[0] = A;
                                                              s[1] = B;
                                                              s[2] = C;
                                                              s[3] = D;
                                                              s[4] = E;
                                                              s[5] = F;
                                                              s[6] = G;
                                                              s[7] = H;
                                                              s[8] = I;
                                                              s[9] = J;
                                                              s[10] = '\0';
                                    
                                                              rotate(main_ctx,a, b, c, d, e, cyph, s, ct);
                                                          }
                                                      }
                                                  }
                                                  else{
                                                      rotate(main_ctx,a, b, c, d, e, cyph, s, ct);
                                                  }
                                              }
                                          }
                                      }
                                      else{
                                          rotate(main_ctx,a, b, c, d, e, cyph, s, ct);
                                      }
                                  }
                              }
                          }
                          else{
                              rotate(main_ctx,a, b, c, d, e, cyph, s,  ct);
                          }
                      }
                  }
              }
              else{
                  rotate(main_ctx,a, b, c, d, e, cyph, s, ct);
                    }
                }
            }
        }
    }
    const time_t proc_stop = time (NULL);
    printf("Time elapsed Test: begin %ld - end %ld \n",proc_start,proc_stop);
}

/*run on all permutations of wheels a, b, c, d, e*/
void permute(main_ctx_t *main_ctx,int a, int b, int c, int d, int e, char *cyph, int *ct)
{
    printf("... testing\n");
    test(main_ctx,a, b, c, d, e, cyph, ct);
    test(main_ctx,b, a, c, d, e, cyph, ct);
    test(main_ctx,b, c, a, d, e, cyph, ct);
    test(main_ctx,b, c, d, a, e, cyph, ct);
    test(main_ctx,b, c, d, e, a, cyph, ct);
    test(main_ctx,b, c, d, e, a, cyph, ct);
    test(main_ctx,c, b, d, e, a, cyph, ct);
    test(main_ctx,c, d, b, e, a, cyph, ct);
    test(main_ctx,c, d, e, b, a, cyph, ct);
    test(main_ctx,c, d, e, a, b, cyph, ct);
    test(main_ctx,c, d, e, a, b, cyph, ct);
    test(main_ctx,d, c, e, a, b, cyph, ct);
    test(main_ctx,d, e, c, a, b, cyph, ct);
    test(main_ctx,d, e, a, c, b, cyph, ct);
    test(main_ctx,d, e, a, b, c, cyph, ct);
    test(main_ctx,e, d, a, b, c, cyph, ct);
    test(main_ctx,e, a, d, b, c, cyph, ct);
    test(main_ctx,e, a, b, d, c, cyph, ct);
    test(main_ctx,e, a, b, c, d, cyph, ct);
    test(main_ctx,a, e, b, c, d, cyph, ct);
    test(main_ctx,a, b, e, c, d, cyph, ct);
    test(main_ctx,a, b, c, e, d, cyph, ct);
}
/*all combinations of five possible wheels*/
void permuteAll(main_ctx_t *main_ctx,char *cyph)
{
    int ct = 0;
  for(int d = 1;d<=9;d++){
      for(int e = 1;e<=9;e++){
          for(int f = 1;f<=9;f++){
             for(int g = 1;g<=9;g++){
                for(int h = 1;h<=9;h++){
                    permute(main_ctx,d,e,f,g,h, cyph, &ct);
                }
             }
          }
      }
  }
    printf("\n... Found %d solutions.\n", ct);
}
/*once combination of five possible wheels*/
void permuteOnce(main_ctx_t *main_ctx,int a, int b, int c, int d, int e, char *cyph)
{
  int ct = 0;
    permute(main_ctx,a, b, c, d, e, cyph, &ct);
    printf("\n... Found %d solutions.\n", ct);
}
/*helper to read a character*/
char readCh(void)
{
    char c, ret = '\0';
  while((c = getchar()) != '\n')
  ret = c;
  return ret;
}
/*init the starting position*/
void initParams(main_ctx_t *main_ctx)
{
  int i,k,j;
  char c,d,g;
  printf("d)efault or u)ser: ");
  c = readCh();
  if(c != 'u')
  {
      if(strcmp(nerd, "--option-1") == 0)
      {
          strlcpy(ukw,ref1,MSGC);
          strlcpy(nox,notch1,MSGG);
      }
      for(i = 0; i < 5; i++)
      {
        printf("Wheel %d: ", i + 1);
        main_ctx->order[i] = readCh() - 48;
      }
      for(i = 0; i < 5; i++)
      {
        printf("Start %d: ", i + 1);
        main_ctx->pos[i] = readCh();
      }
      for(i = 0; i < 5; i++)
      {
        printf("Ring  %d: ", i + 1);
        main_ctx->rings[i] = readCh();
      }
      printf("Stecker: ");
      i = 0;
      while((d = getchar()) != '\n')
      {
        main_ctx->plug[i] = d;
        i++;
      }
      main_ctx->plug[i] = '\0';
  }
  else
  {
      printf("Reflector: ");
      k = 0;
      while((d = getchar()) != '\n')
      {
      ukw[k] = d;
      k++;
      }
      ukw[k] = '\0';
      
      printf("NOTCH: ");
      j = 0;
      while((g = getchar()) != '\n')
      {
      nox[j] = g;
      j++;
      }
      nox[j] = '\0';
      
      for(i = 0; i < 5; i++)
      {
        printf("Wheel %d: ", i + 1);
        main_ctx->order[i] = readCh() - 48;
      }
      for(i = 0; i < 5; i++)
      {
        printf("Start %d: ", i + 1);
        main_ctx->pos[i] = readCh();
      }
      for(i = 0; i < 5; i++)
      {
        printf("Ring  %d: ", i + 1);
        main_ctx->rings[i] = readCh();
      }
      printf("Stecker: ");
      i = 0;
      while((c = getchar()) != '\n')
      {
        main_ctx->plug[i] = c;
        i++;
      }
      main_ctx->plug[i] = '\0';
        
  }
  printf("\x1B[33mWheels\x1B[39m \x1B[32m %d %d %d %d %d \x1B[39m \x1B[33mStart\x1B[39m \x1B[32m %c %c %c %c %c \x1B[39m \x1B[33mRings\x1B[39m \x1B[32m %c %c %c %c %c \x1B[39m Stecker \"\x1B[32m%s\x1B[39m\"\n\x1B[33mReflector\x1B[39m \x1B[32m %s \x1B[39m \x1B[33mNOTch\x1B[39m\x1B[32m %s \x1B[39m\n",
         main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4],
         main_ctx->pos[0], main_ctx->pos[1], main_ctx->pos[2], main_ctx->pos[3], main_ctx->pos[4],
         main_ctx->rings[0], main_ctx->rings[1], main_ctx->rings[2], main_ctx->rings[3], main_ctx->rings[4], main_ctx->plug,ukw,nox);
}

void sbfParams(main_ctx_t *main_ctx)
{
    hashcat_ctx_t *hashcat_ctx = NULL;
    
  int i,j,k,l;
  char f,a,b,d,g;
  printf("d)efault or u)ser: ");
  f = readCh();
  if(f != 'u')
  {
      if(strcmp(nerd, "--option-1b") == 0)
      {
          strlcpy(ukw,ref1,MSGC);
          strlcpy(nox,notch1,MSGG);
      }
      for(i = 0; i < 5; i++)
      {
        printf("Wheel %d: ", i + 1);
        main_ctx->order[i] = readCh() - 48;
      }
      
      printf("Message: ");
      i = 0;
      while((a = getchar()) != '\n')
      {
      main_ctx->cyph[i] = a;
      i++;
      }
      main_ctx->cyph[i] = '\0';
      
      printf("Dict: ");
      l = 0;
      while((b = getchar()) != '\n')
      {
      framex[l] = b;
      l++;
      }
      framex[l] = '\0';
  }
  else
  {
    printf("Reflector: ");
    j = 0;
    while((d = getchar()) != '\n')
    {
    ukw[j] = d;
    j++;
    }
    ukw[j] = '\0';
    if(strcmp(ukw,"xxxxx") == 0)
    {
          strlcpy(ukw,coinr(),MSGC);
    }
    printf("NOTCH: ");
    k = 0;
    while((g = getchar()) != '\n')
    {
    nox[k] = g;
    k++;
    }
    nox[k] = '\0';
    if(strcmp(nox,"xxxxx") == 0)
    {
          strlcpy(nox,coinn(),MSGG);
    }
    for(i = 0; i < 5; i++)
    {
    printf("Wheel %d: ", i + 1);
    main_ctx->order[i] = readCh() - 48;
    }
    printf("Message: ");
    i = 0;
    while((a = getchar()) != '\n')
    {
    main_ctx->cyph[i] = a;
    i++;
    }
    main_ctx->cyph[i] = '\0';
    
    printf("Dict: ");
    l = 0;
    while((b = getchar()) != '\n')
    {
    framex[l] = b;
    l++;
    }
    framex[l] = '\0';
        
  }
    printf("\x1B[33mWheels\x1B[39m \x1B[32m %d %d %d %d %d \x1B[39m \x1B[33mMessage\x1B[39m\x1B[32m %s \x1B[39m\x1B[33mDict\x1B[39m \x1B[32m %s \x1B[39m\n\x1B[33mReflector\x1B[39m \x1B[32m %s \x1B[39m \x1B[33mNOTch\x1B[39m\x1B[32m %s \x1B[39m\n",
           main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4], main_ctx->cyph, framex,ukw,nox);
    hashcat_session_execute (hashcat_ctx);
}

void bfParams(main_ctx_t *main_ctx)
{
    hashcat_ctx_t *hashcat_ctx = NULL;
    
  int i,j,k,l;
  char c,a,b,d,g;
  printf("d)efault or u)ser: ");
  c = readCh();
  if(c != 'u')
  {
      if(strcmp(nerd, "--option-1a") == 0)
      {
          strlcpy(ukw,ref1,MSGC);
          strlcpy(nox,notch1,MSGG);
      }
      printf("Message: ");
      i = 0;
      while((a = getchar()) != '\n')
      {
      main_ctx->cyph[i] = a;
      i++;
      }
      main_ctx->cyph[i] = '\0';
      
      printf("Dict: ");
      l = 0;
      while((b = getchar()) != '\n')
      {
      framex[l] = b;
      l++;
      }
      framex[l] = '\0';
          
  }
  else
  {
      printf("Reflector: ");
      j = 0;
      while((d = getchar()) != '\n')
      {
      ukw[j] = d;
      j++;
      }
      ukw[j] = '\0';
      if(strcmp(ukw,"xxxxx") == 0)
      {
            strlcpy(ukw,coinr(),MSGC);
      }
      printf("NOTCH: ");
      k = 0;
      while((g = getchar()) != '\n')
      {
      nox[k] = g;
      k++;
      }
      nox[k] = '\0';
      if(strcmp(nox,"xxxxx") == 0)
      {
            strlcpy(nox,coinn(),MSGG);
      }
      printf("Message: ");
      i = 0;
      while((a = getchar()) != '\n')
      {
      main_ctx->cyph[i] = a;
      i++;
      }
      main_ctx->cyph[i] = '\0';
      
      printf("Dict: ");
      l = 0;
      while((b = getchar()) != '\n')
      {
      framex[l] = b;
      l++;
      }
      framex[l] = '\0';
        
  }
    printf("\x1B[33mMessage\x1B[39m\x1B[32m %s \x1B[39m\x1B[33mDict\x1B[39m \x1B[32m %s \x1B[39m\n\x1B[33mReflector\x1B[39m \x1B[32m %s \x1B[39m \x1B[33mNOTch\x1B[39m\x1B[32m %s \x1B[39m\n",
           main_ctx->cyph, framex,ukw,nox);
    hashcat_session_execute (hashcat_ctx);
}

/********************************************MAIN*********************************************/
int main(int argc, char **argv) {
    main_ctx_t main_ctx;
    hashcat_ctx_t *hashcat_ctx = NULL;
    
    if(argc < 2 || argc > 4){ /*main case*/
          printf("\x1b[32m");
          printf("\nOption usage: %s --help\n",argv[0]);
          printf("\x1b[0m");
          return -1;
      }
    
        hashcat_session_init (hashcat_ctx, "~/bin", "~/share", argc, argv,atoi("01-01-2023 00:00:01"));
        // now execute hashcat
        backend_info_compact (hashcat_ctx);
        
        user_options_info (hashcat_ctx);
    
        if(strcmp(argv[1], "--option-1a") == 0)
        {
            printf("Config File: ");
            char *b;
            int l = 0;
            while((b = getchar()) != '\n')
            {
            flames[l] = b;
            l++;
            }
            flames[l] = '\0';
            strcpy(nerd,argv[1]);
            configmain(flames);
            printf("Option 1\n");
            bfParams(&main_ctx);
            permuteAll(&main_ctx,main_ctx.cyph);
        }
        if(strcmp(argv[1], "--option-1b") == 0)
        {
            printf("Config File: ");
            char *b;
            int l = 0;
            while((b = getchar()) != '\n')
            {
            flames[l] = b;
            l++;
            }
            flames[l] = '\0';
            strcpy(nerd,argv[1]);
            configmain(flames);
            printf("Option 1\n");
            sbfParams(&main_ctx);
            permuteOnce(&main_ctx,main_ctx.order[0], main_ctx.order[1],main_ctx.order[2], main_ctx.order[3], main_ctx.order[4],main_ctx.cyph);
        }
        if(strcmp(argv[1], "--option-1") == 0)
        {
            printf("Config File: ");
            char *b;
            int l = 0;
            while((b = getchar()) != '\n')
            {
            flames[l] = b;
            l++;
            }
            flames[l] = '\0';
            strcpy(nerd,argv[1]);
            configmain(flames);
            printf("Enigma\n");
            initParams(&main_ctx);
            cypher(main_ctx);
        }
        if(strcmp(argv[1], "--version") == 0)
        {
            printf("Version\n\n\t\x1B[35m%s\x1B[39m -  \x1B[32mT.E.D.\x1B[39m - \x1B[33mThe Enemy Dail\x1B[39m - Koenig Martin\n",PROGNAME);
        }
        if(strcmp(argv[1], "--help") == 0)
        {
            printf("Help\n\n\t\x1B[33m--option-1a -T8 -D2\x1B[39m = Enigma Crack Algo\n\t\x1B[33m--option-1b -T8 -D2\x1B[39m = Enigma Crack Once Algo\n\t\x1B[33m--option-1\x1B[39m = Enigma 5 Rotor Calculator\n\t\x1B[33m--version\x1B[39m = Version\n\n");
        }
    
    return 0;
}
