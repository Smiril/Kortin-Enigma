/* Original author: James Lyons
Aug 2012
use e.g. http://practicalcryptography.com/ciphers/mechanical-era/enigma/ to generate messages
this code is from http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma/
Modifications author: Smiril
 Mar 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "enigma.h"
// Rotors
char key[16][26];
// Reflectors, M3 enigma always uses second reflector
char refl[3][26];
// notches indicate where the rotors increment. rotors 6-8 have two notches
//  to make the first 5 rotors the same, we assume both notches are at the same spot
char notch[8][2];

void configmain(char *docname) {
   
    xmlDocPtr       doc;
    xmlNodePtr      cur;
    xmlChar         *uri;
    int countz = 0;
    int county = 0;
    int countx = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    //int count2 = 0;
    char *config1[16][7] = { {
        "rotor1", "rotor2", "rotor3", "rotor4", "rotor5", "rotor6", "rotor7", "rotor8","rotora", "rotorb", "rotorc", "rotord", "rotore", "rotorf", "rotorg", "rotorh"} };
    char *config2[3][7] = { {"refle1", "refle2", "refle3"} };
    char *config3[8][7] = { {
        "notch1", "notch2", "notch3", "notch4", "notch5", "notch6", "notch7", "notch8"} };
    printf("Loading config file: %s\n",docname);
    FILE *fp;
    fp = fopen(docname,"r");
    struct stat st;
    fstat(fileno(fp), &st);
    size_t size = st.st_size;
    
    if (1547 != size) {
        printf("config file is corrupt\n");
        exit(1);
    }
    
    fclose(fp);
    
    doc = xmlParseFile(docname);
    cur = xmlDocGetRootElement(doc);
    
    if((xmlStrcmp(cur->name, (const xmlChar *) "um-configuration")) != 0) {
        printf("error wrong file!\n");
        exit(1);
    }
    
    xmlNode* child = cur->children->next;
    
    // get the child's attributes
    xmlAttr* attr = child->properties;
    while (attr != NULL) {
        if((xmlStrcmp(attr->name, (const xmlChar *) "config")) != 0) {
        printf("error wrong file!\n");
        exit(1);
    }
      // move to the next attribute
      attr = attr->next;
    }

    // go through the child's children
    xmlNode* gchild = child->children->next;
    while (gchild != NULL) {
            if (xmlStrcmp(gchild->name, (const xmlChar *)config1[county++])) {
                if((uri =  xmlGetProp(gchild,(const xmlChar *)"name")) != NULL) {
                    strcpy(key[count1++],(const char *)uri);
                    //printf("%s\n",uri);
                    xmlFree(uri);
                }
            }
            else if (xmlStrcmp(gchild->name, (const xmlChar *)config2[countx++])) {
                if((uri =  xmlGetProp(gchild,(const xmlChar *)"name")) != NULL) {
                    strcpy(refl[count2++],(const char *)uri);
                    //printf("%s\n",uri);
                    xmlFree(uri);
                }
            }
            else if (xmlStrcmp(gchild->name, (const xmlChar *)config3[countz++])) {
                if((uri =  xmlGetProp(gchild,(const xmlChar *)"name")) != NULL) {
                    strcpy(notch[count3++],(const char *)uri);
                    //printf("%s\n",uri);
                    xmlFree(uri);
                }
            }
        gchild = gchild->next->next;
    }
    xmlFreeDoc(doc);
}

/*********************************************************
enigma - encipher (or decipher) a string 'from', put the result in 'to'
- note that the key will be modified by this function (indicator settings are incremented)
**********************************************************/
char enigma(EnigmaKey *key, char* from, char* to){
    unsigned long i;
    for(i=0;i<strlen(from);i++){
        to[i] = enigma_encipher(from[i],key);
    }
    to[i] = '\0'; //null terminate
    return *to;
}

/*********************************************************
enigma_encipher - encipher (or decipher) a single character
- note that the settings array will be modified by this function
**********************************************************/
char enigma_encipher(char ch, EnigmaKey *key){
//int settings[3], char ringstellung[3], Reflector r, Rotor rotors[3], char steckers[13][2])
    ch = toupper(ch);
    // increment the settings before the current letter is enciphered
    increment_indicator_settings(key->indicator, key->rotors);

    ch = apply_steckers(ch,key->plugboard);
    // encipher the current character going through rotors right to left
    ch=rotor(key->rotors[4],ch,key->indicator[4]-key->ringsettings[4]);
    ch=rotor(key->rotors[3],ch,key->indicator[3]-key->ringsettings[3]);
    ch=rotor(key->rotors[2],ch,key->indicator[2]-key->ringsettings[2]);  
    ch=rotor(key->rotors[1],ch,key->indicator[1]-key->ringsettings[1]);
    ch=rotor(key->rotors[0],ch,key->indicator[0]-key->ringsettings[0]);  
    // ch gets to the reflector...
    ch=reflector(key->reflector,ch); 
    // now ch goes back through the rotors from left to right (inverse substitution)
    ch=rotor(inverse(key->rotors[0]),ch,key->indicator[0]-key->ringsettings[0]);  
    ch=rotor(inverse(key->rotors[1]),ch,key->indicator[1]-key->ringsettings[1]);   
    ch=rotor(inverse(key->rotors[2]),ch,key->indicator[2]-key->ringsettings[2]);   
    ch=rotor(inverse(key->rotors[3]),ch,key->indicator[3]-key->ringsettings[3]);
    ch=rotor(inverse(key->rotors[4]),ch,key->indicator[4]-key->ringsettings[4]);

    ch = apply_steckers(ch,key->plugboard);   

    return ch;
} 

/*********************************************************
apply_steckers - performs the letter substitutions
 - it is the user's resposibility to ensure there are no
    duplicates in the substitution list
**********************************************************/
char apply_steckers(char in, char steckers[13][2]){
    int i;
    for(i=0;i<13;i++){
        if(in==steckers[i][0]) return steckers[i][1];
        else if(in==steckers[i][1]) return steckers[i][0];
    }
    return in;
}


/***********************************************************
 Given a rotor, return the inverse 
 ***********************************************************/
Rotor inverse(Rotor r){
    switch(r){
    case ROTOR_I:    return ROTOR_I_INV; 
    case ROTOR_II:   return ROTOR_II_INV;
    case ROTOR_III:  return ROTOR_III_INV;
    case ROTOR_IV:   return ROTOR_IV_INV;
    case ROTOR_V:    return ROTOR_V_INV;
    case ROTOR_VI:   return ROTOR_VI_INV;
    case ROTOR_VII:  return ROTOR_VII_INV;
    case ROTOR_VIII: return ROTOR_VIII_INV;
    default: return -1;
    }
}

/*********************************************************
ROTOR - from ENIGMA I - http://en.wikipedia.org/wiki/Enigma_rotor_details
- Rotor chooses which rotor to use
- assumes char in is an uppercase char A-Z
**********************************************************/
char rotor(Rotor rotor, char in, int offset){
    return (((key[rotor][(in-'A'+26+offset)%26] -offset+26-'A') %26+'A' -offset+26-'A') %26+'A' -offset+26-'A') %26+'A';
}
/*********************************************************
REFLECTOR - from ENIGMA I - http://en.wikipedia.org/wiki/Enigma_rotor_details
- r chooses which reflector to use
- assumes char in is an uppercase char A-Z
**********************************************************/
char reflector(Reflector r, char in){
     return refl[r][in-'A'];
}

/****************************************************************************
advances the rotors depending on which rotors are in which position
- uses notch array, which records when each rotor increments the one to the left
*****************************************************************************/
void increment_indicator_settings(int settings[5], Rotor r[5]){
    if(settings[1]+'A' == notch[r[1]][0] || settings[1]+'A' == notch[r[1]][1]){
        settings[0] = (settings[0]+1)%26;
        settings[1] = (settings[1]+1)%26;
        settings[2] = (settings[2]+1)%26;
        settings[3] = (settings[3]+1)%26;
    }
    if(settings[1]+'A' == notch[r[1]][0] || settings[1]+'A' == notch[r[1]][1]){
        settings[0] = (settings[0]+1)%26;
        settings[1] = (settings[1]+1)%26;
        settings[2] = (settings[2]+1)%26;
    }
    if(settings[1]+'A' == notch[r[1]][0] || settings[1]+'A' == notch[r[1]][1]){
        settings[0] = (settings[0]+1)%26;
        settings[1] = (settings[1]+1)%26;
    }    
    if(settings[2]+'A' == notch[r[2]][0] || settings[2]+'A' == notch[r[2]][1]){
        settings[1] = (settings[1]+1)%26;
    }
    settings[4] = (settings[4]+1)%26;
}

/****************************************************************************
find an empty spot in the plugboard and add (c1,c2) as an entry
returns -1 if no room to add, 0 on success
*****************************************************************************/
int appendToPlugboard(EnigmaKey *key,char c1, char c2){
    int i;
    // if either of the chars is already connected, don't add it
    for(i=0;i<13;i++){
        if((key->plugboard[i][0]==c1) || (key->plugboard[i][0]==c2)){
            return -1;
        }else if((key->plugboard[i][1]==c1) || (key->plugboard[i][1]==c2)){
            return -1;
        }
    }
    for(i=0;i<13;i++){
        if((key->plugboard[i][0]<0) && (key->plugboard[i][1]<0)){
            key->plugboard[i][0] = c1;
            key->plugboard[i][1] = c2;
            return 0;
        }
    }
    return -1;
}

/****************************************************************************
print a given key, useful for debugging
*****************************************************************************/
void printEnigmaKey(EnigmaKey *key){
    int i;
    printf("indicator=%c%c%c%c%c, ",key->indicator[0]+'A',key->indicator[1]+'A',key->indicator[2]+'A',key->indicator[3]+'A',key->indicator[4]+'A');
    printf("rotors=%d%d%d%d%d, ",key->rotors[0]+1,key->rotors[1]+1,key->rotors[2]+1,key->rotors[3]+1,key->rotors[4]+1);
    printf("rings=%c%c%c%c%c, plugboard= ",key->ringsettings[0]+'A',key->ringsettings[1]+'A',key->ringsettings[2]+'A',key->ringsettings[3]+'A',key->ringsettings[4]+'A');
    for(i=0;i<13;i++){
        if(isalpha(key->plugboard[i][0]) && isalpha(key->plugboard[i][1])){
            printf("%c%c ",key->plugboard[i][0],key->plugboard[i][1]);
        }else break;
    }
    printf("\n");
}

/****************************************************************************
Initialise a key, with default information
*****************************************************************************/
void initEnigmaKey(EnigmaKey *key){
    int i;
    key->indicator[0] = 0;
    key->indicator[1] = 0;
    key->indicator[2] = 0;
    key->indicator[3] = 0;
    key->indicator[4] = 0;
    key->reflector=REFLECTOR_A;
    key->ringsettings[0]=0;
    key->ringsettings[1]=0;
    key->ringsettings[2]=0;
    key->ringsettings[3]=0;
    key->ringsettings[4]=0;
    for(i=0;i<13;i++){
        key->plugboard[i][0] = -1;
        key->plugboard[i][1] = -1;
    }
    key->rotors[0] = 0;
    key->rotors[1] = 1;
    key->rotors[2] = 2;
    key->rotors[3] = 3;
    key->rotors[4] = 4;
}
