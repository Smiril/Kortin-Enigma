/*****************************************************************************
*
* This source code is distributed under the MIT license, COPYING.
*
* Author: Smiril <sonar@gmx.com>
*
******************************************************************************/
#define CA_PRIVATE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define NUM_ROTORS 5
#define MSGLEN 160

char *FINC;
char *notch1;
char *flames;

// Structure to hold the Enigma machine parameters
typedef struct P{
    char rotor[NUM_ROTORS][27];
    char reflec[27];
    char notch[6];
    char pos[6];
    char rings[6];
    int order[6];
    char plug[27];
} Params;

Params p;

const char* Versionx(void) {
#ifdef VERSION
    return VERSION;
#else
  return "WarGames 0.5 T.E.D. - The Enemy Dial - ENEMY-MODE";
#endif
}

// load xmlStructure to the Enigma machine parameters
void configmain(Params *p,char *docname) {
    
    xmlDocPtr       doc;
    xmlNodePtr      cur;
    const unsigned char *uri;
    //int count = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    char *config[5][6] = { {"rotor1", "rotor2", "rotor3", "rotor4", "rotor5"} };
    char *configx[1][6] = { {"refle1"} };
    char *configy[1][8] = { {"xmlnotch"} };
    
    printf("Loading config file: %s\n",docname);
    FILE *fp;
    fp = fopen(docname,"r");
    struct stat st;
    fstat(fileno(fp), &st);
    size_t size = st.st_size;
    
    if (522 != size) {
        printf("config file is corrupt\n");
        fclose(fp);
        exit(1);
    }
    
    doc = xmlParseFile(docname);
    cur = xmlDocGetRootElement(doc);
    
    if((xmlStrcmp(cur->name, (const xmlChar *) "um-configuration")) != 0) {
        printf("error wrong file!\n");
        fclose(fp);
        exit(1);
    }
    
    xmlNode* child = cur->children->next;
    
    // get the child's attributes
    xmlAttr* attr = child->properties;
    while (attr != NULL) {
        if((xmlStrcmp(attr->name, (const xmlChar *) "config")) != 0) {
            printf("error wrong file!\n");
            fclose(fp);
            exit(1);
        }
        // move to the next attribute
        attr = attr->next;
    }
    
    // go through the child's children
    xmlNode* gchild = child->children->next;
    while (gchild != NULL) {
        if (xmlStrcmp(gchild->name, (const xmlChar *)config[count1++])) {
            if((uri = xmlGetProp(gchild,(const xmlChar *)"name")) != NULL) {
                strcpy((char *)p->rotor[count1],(const char *)uri);
                xmlFree(uri);
            }
        }
        else if (xmlStrcmp(gchild->name, (const xmlChar *)configx[count2++])) {
            if((uri = xmlGetProp(gchild,(const xmlChar *)"name")) != NULL) {
                strcpy((char *)p->reflec,(const char *)uri);
                xmlFree(uri);
            }
        }
        else if (xmlStrcmp(gchild->name, (const xmlChar *)configy[count3++])) {
            if((uri = xmlGetProp(gchild,(const xmlChar *)"name")) != NULL) {
                strcpy((char *)p->notch,(const char *)uri);
                xmlFree(uri);
            }
        }
        gchild = gchild->next->next;
    }
    xmlFreeDoc(doc);
    fclose(fp);
}

// Function to rotate the rotor
void rotateRotor(char *rotor) {
    char temp = rotor[0];
    if (strcmp(FINC, "d") == 0) {
        for (int i = 26; i >= 0; i--) {
            rotor[i] = rotor[i + 1];
        }
    }
    if (strcmp(FINC, "e") == 0) {
        for (int i = 0; i < 26; i++) {
            rotor[i] = rotor[i + 1];
        }
    }
    rotor[26] = temp;
}

// Function to perform the Enigma encryption or decryption
char enigmaTransform(char input, char rotors[NUM_ROTORS][27]) {
    // Pass the input character through the rotors
    if (strcmp(FINC, "d") == 0) {
        for (int i = NUM_ROTORS -1; i >= 0; i--) {
            input = rotors[i][input - 'A'];
        }
    }
    if (strcmp(FINC, "e") == 0) {
        for (int i = 0; i < NUM_ROTORS; i++) {
            input = rotors[i][input - 'A'];
        }
    }
    // Pass the input character through the reflector
    input = 'Z' - (input - 'A');

    // Pass the input character through the rotors in reverse order
    if (strcmp(FINC, "d") == 0) {
        for (int i = NUM_ROTORS - 1; i >= 0; i--) {
            for (int j = 26; j >= 0; j--) {
                for (int k = 0; p.plug[k]; k += 2) {
                    if (input == p.plug[k])
                        input = p.plug[k + 1];
                    else if (input == p.plug[k + 1])
                        input = p.plug[k];
                }
                
                for (int l = 0;l<=5;l++){
                    if (p.notch[l] == input) {
                        input = j + 1;
                    }
                }

                if (rotors[i][j] == input) {
                    input = j + p.reflec[j];
                    break;
                }
            }
        }
    }
    if (strcmp(FINC, "e") == 0) {
        for (int i = 0 ; i < NUM_ROTORS; i++) {
            for (int j = 0; j < 26; j++) {
                for (int k = 0; p.plug[k]; k += 2) {
                    if (input == p.plug[k])
                        input = p.plug[k + 1];
                    else if (input == p.plug[k + 1])
                        input = p.plug[k];
                }
                
                for (int l = 0;l<=5;l++){
                    if (p.notch[l] == input) {
                        input = j + 1;
                    }
                }

                if (rotors[i][j] == input) {
                    input = j + p.reflec[j];
                    break;
                }
            }
        }
    }
    return input;
}

// Function to decipher a message
char *decipher(Params *p, char *message) {
    // Decrypt each character in the message
    for (unsigned long int i = 0; i < strlen(message); i++) {
        char input = message[i];
        if (isalpha(input)) {
            // Decrypt the input character
            message[i] = enigmaTransform(input, p->rotor);

            if (strcmp(FINC, "d") == 0) {
                // Rotate the rotors after each transformation
                for (int j = NUM_ROTORS - 1; j >= 0; j--) {
                    rotateRotor(p->rotor[j]);
                }
            }
            if (strcmp(FINC, "e") == 0) {
                // Rotate the rotors after each transformation
                for (int j = 0; j < NUM_ROTORS; j++) {
                    rotateRotor(p->rotor[j]);
                }
            }
        }
    }
    return message;
}


/*helper to read a character*/
char readCh(void)
{
    char c, ret = '\0';
    while((c = getchar()) != '\n')
        ret = c;
    return ret;
}

char *revstr (char *st)
{
    int len, i;
    char *start, *end, temp;
      
    len = strlen(st);
    start = st;
    end = st;
      
    for (i = 0; i < len - 1; i++)
    end++;
      
    for (i = 0; i < len/2; i++)
    {
        temp = *end;
        *end = *start;
        *start = temp;
          
        start++;
        end--;
    }
    return st;
}

/*init the starting position*/
int initParams(Params *p)
{
    int i;
    char c,d;
    printf("e)ncrypt or d)ecrypt: ");
    c = readCh();
    if(c == 'd')
    {
        FINC = "d";
        //notch1 = revstr(notch);
        for(i = 0; i < 5; i++)
        {
            printf("Wheel %d: ", i + 1);
            p->order[i] = (readCh() - 48);
        }
        for(i = 0; i < 5; i++)
        {
            printf("Start %d: ", i + 1);
            p->pos[i] = readCh();
        }
        for(i = 0; i < 5; i++)
        {
            printf("Ring  %d: ", i + 1);
            p->rings[i] = readCh();
        }
        printf("Stecker: ");
        i = 0;
        while((d = getchar()) != '\n')
        {
            p->plug[i] = d;
            i++;
        }
        p->plug[i] = '\0';
    }
    else if (c == 'e')
    {
        FINC = "e";
        //notch1 = notch;
        for(i = 0; i < 5; i++)
        {
            printf("Wheel %d: ", i + 1);
            p->order[i] = (readCh() - 48);
        }
        for(i = 0; i < 5; i++)
        {
            printf("Start %d: ", i + 1);
            p->pos[i] = readCh();
        }
        for(i = 0; i < 5; i++)
        {
            printf("Ring  %d: ", i + 1);
            p->rings[i] = readCh();
        }
        printf("Stecker: ");
        i = 0;
        while((d = getchar()) != '\n')
        {
            p->plug[i] = d;
            i++;
        }
        p->plug[i] = '\0';
    }
    else {
        printf("\a\a\aError\n");
        return -1;
    }
    printf("Wheels %d %d %d %d %d Start %c %c %c %c %c Rings %c %c %c %c %c Stecker \"%s\"\n",
           p->order[0], p->order[1], p->order[2], p->order[3], p->order[4],
           p->pos[0], p->pos[1], p->pos[2], p->pos[3], p->pos[4],
           p->rings[0], p->rings[1], p->rings[2], p->rings[3], p->rings[4], p->plug);
    return 0;
}


int main(void) {
    // Create a Params structure with default values
    char a1;
    int a = 0;
    printf("Config File: ");
    a = 0;
    while((a1 = getchar()) != '\n')
    {
        flames[a] = a1;
        a++;
    }
    flames[a] = '\0';
    
    configmain(&p,flames);

    initParams(&p);

    // Encrypted message to decrypt
    char inx[MSGLEN];
    char c;
    int i = 0;
    while ((c = getchar()) != '\n') {
        if (isspace(c)) {
            c = 'X';
        }
        inx[i] = toupper(c);
        i++;
    }
    inx[i] = '\0';

    // Decrypt the message
    char *Message = decipher(&p, inx);

    // Output the decrypted message
    printf("Message: %s\n", Message);
    printf("Version: %s\n", Versionx());
    return 0;
}
