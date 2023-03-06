/* author: James Lyons
Aug 2012
this code is from http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma/
 Modifications author: Smiril
  Mar 2023
*/
#include "scoreText.h"
#include "de_qgr.h"

extern float qgram[];

// assumes that text consists only of uppercase letters(no punctuation or spaces)
unsigned long long int scoreTextQgram(char *text,int len){
    int i;
    char temp[5];
    unsigned long long int score = 0;
    for (i=0;i<len-5;i++){
        temp[0]=text[i]-'A';
        temp[1]=text[i+1]-'A';
        temp[2]=text[i+2]-'A';
        temp[3]=text[i+3]-'A';
        temp[4]=text[i+4]-'A';
        score += qgram[308915776U*temp[0] + 17576U*temp[1] + 676U*temp[2] + 26U*temp[3] + temp[4]];
    }
    return score;
}
// scoreText with known plaintext, counts number of identical characters
unsigned long long int scoreTextKP(char *text1,char *text2,int len){
    int i;
    unsigned long long int score = 0;
    for (i=0;i<len;i++){
        if (text1[i] == text2[i]) score += 1;
    }
    return score;
}
