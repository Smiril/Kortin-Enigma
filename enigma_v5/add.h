//
//  add.h
//  enigma
//
//  Created by Smiril on 20.07.22.
//  Copyright © 2022 Smiril. All rights reserved.
//

#ifndef add_h
#define add_h
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#if defined(__WIN32__) && defined(__WIN64__)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netdb.h>
#endif
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/tree.h>
#include <libxml/threads.h>
#include <signal.h>
// on troubles openssl see: https://stackoverflow.com/questions/69002453/how-to-build-openssl-for-m1-and-for-intel
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/tls1.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) {perror(s);exit (1);}
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit (2); }

#define SERVICE "https" // possible http https
#define PROTO "tcp" // possible tcp udp icmp igmp raw

#define USERAGENT "Enigma/0.0.5 (iPad;CPU OS 13_1_2 like Mac OS X) AppleWebKit/602.2.14 (KHTML, like Gecko) Version/13.0 Mobile/14B100 SafaRI/602."

#define DELIM1 "."
#define DELIM2 ":"
#define BUFSIZE 1400
#define BUFSIZ2 4096
#if !defined(__WIN32__) && !defined(__WIN64__)
#define HOME "/usr/local/share/enigma/"
#endif
#if !defined(__APPLE__) && !defined(__LINUX__)
#define HOME "C:\\usr\\local\\share\\enigma\\"
#endif
#define CERTF HOME "valid-root-ca.pem"
#define KEYF HOME "valid-root-cakey.pem"

#define PROGNAME "Enigma"

#define PTHREAD_THREADS_MAX 100

#define IN_RANK 0
#define MAXLINEX 1000000
#define MSGY 1320
#define MSGLEN 4096
#define MSGX 13
#define MSGC 26
#define MSGP 27
#define MSGG 5
#define TO 'Z'



int flag;
int tempres = BUFSIZ2;
int err;
int sd;
int sock2;
const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
struct sockaddr_in sa_serv;
struct sockaddr_in sa;
struct sockaddr_in6 sa_serv6;
struct sockaddr_in6 sa6;
socklen_t client_len;
SSL_CTX* ctx;
SSL* ssl;
X509* client_cert;
X509* server_cert;
char flex[BUFSIZ2];
char* str;
char *jessy0,*jessy1,*jessy2,*jessy3;
char *cora;
const SSL_METHOD *meth;
struct sockaddr_in *remote;
struct sockaddr_in6 *remote6;
int tmpresx;
unsigned long int tmpres;
char *ip;
char *ipp;
char *ipprox;
char *get;
char *get_cl;
char buf[BUFSIZ+1];
char host[MSGY];
char page[MSGY];
char port[MSGG];
char proxy[MSGY];
char proxyport[MSGG];

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char chad[MSGX];
char x[MSGLEN] = "A";
char s[MSGP] = "A";
char fff[MSGLEN] = "A";
char nerd[MSGX];
char framex[MSGY];
char flames[MSGY];
char ukw[MSGC];
char nox[MSGG];
int oldrank = IN_RANK;

int fds[2];

/* NORWAY ENIGMA (after to WWII)
 *
 * Wheel               ABCDEFGHIJKLMNOPQRSTUVWXYZ    Notch    Turnover      #
 * --------------------------------------------------------------------------------------------
 * ETW                 ABCDEFGHIJKLMNOPQRSTUVWXYZ
 * I                   WTOKASUYVRBXJHQCPZEFMDINLG    Y        Q            1
 * II                  GJLPUBSWEMCTQVHXAOFZDRKYNI    M        E            1
 * III                 JWFMHNBPUSDYTIXVZGRQLAOEKC    D        V            1
 * IV                  ESOVPZJAYQUIRHXLNFTGKDCMWB    R        J            1
 * V                   HEJXQOTZBVFDASCILWPGYNMURK    H        Z            1
 * UKW                 MOWJYPUXNDSRAIBFVLKZGQCHET
 * --------------------------------------------------------------------------------------------
 * Rotor #             ABCDEFGHIJKLMNOPQRSTUVWXYZ    Date    Introduced    Model Name & Number
 * --------------------------------------------------------------------------------------------
 * IC                  DMTWSILRUYQNKFEJCAZBPGXOHV    1924    Commercial    Enigma A, B
 * IIC                 HQZGPJTMOBLNCIFDYAWVEUSRKX    1924    Commercial    Enigma A, B
 * IIIC                UQNTLSZFMREHDPXKIBVYGJCWOA    1924    Commercial    Enigma A, B
 * --------------------------------------------------------------------------------------------
 * Rotor #             ABCDEFGHIJKLMNOPQRSTUVWXYZ    Date    Introduced    Model Name & Number
 * --------------------------------------------------------------------------------------------
 * I                   JGDQOXUSCAMIFRVTPNEWKBLZYH    7 February 1941       German Railway (Rocket)
 * II                  NTZPSFBOKMWRCJDIVLAEYUXHGQ    7 February 1941       German Railway (Rocket)
 * III                 JVIUBHTCDYAKEQZPOSGXNRMWFL    7 February 1941       German Railway (Rocket)
 * UKW                 QYHOGNECVPUZTFDJAXWMKISRBL    7 February 1941       German Railway (Rocket)
 * ETW                 QWERTZUIOASDFGHJKPYXCVBNML    7 February 1941       German Railway (Rocket)
 * --------------------------------------------------------------------------------------------
 * Rotor #             ABCDEFGHIJKLMNOPQRSTUVWXYZ    Date Introduced       Model Name & Number
 * --------------------------------------------------------------------------------------------
 * I-K                 PEZUOHXSCVFMTBGLRINQJWAYDK    February 1939         Swiss K
 * II-K                ZOUESYDKFWPCIQXHMVBLGNJRAT    February 1939         Swiss K
 * III-K               EHRVXGAOBQUSIMZFLYNWKTPDJC    February 1939         Swiss K
 * UKW-K               IMETCGFRAYSQBZXWLHKDVUPOJN    February 1939         Swiss K
 * ETW-K               QWERTZUIOASDFGHJKPYXCVBNML    February 1939         Swiss K
 * --------------------------------------------------------------------------------------------
 * Rotor #             ABCDEFGHIJKLMNOPQRSTUVWXYZ    Date Introduced       Model Name & Number
 * --------------------------------------------------------------------------------------------
 * I                   EKMFLGDQVZNTOWYHXUSPAIBRCJ    1930                  Enigma I
 * II                  AJDKSIRUXBLHWTMCQGZNPYFVOE    1930                  Enigma I
 * III                 BDFHJLCPRTXVZNYEIWGAKMUSQO    1930                  Enigma I
 * IV                  ESOVPZJAYQUIRHXLNFTGKDCMWB    December 1938         M3 Army
 * V                   VZBRGITYUPSDNHLXAWMJQOFECK    December 1938         M3 Army
 * VI                  JPGVOUMFYQBENHZRDKASXLICTW    1939                  M3 & M4 Naval (FEB 1942)
 * VII                 NZJHGRCXMYSWBOUFAIVLPEKQDT    1939                  M3 & M4 Naval (FEB 1942)
 * VIII                FKQHTLXOCBJSPDZRAMEWNIUYGV    1939                  M3 & M4 Naval (FEB 1942)
 * --------------------------------------------------------------------------------------------
 * Rotor #             ABCDEFGHIJKLMNOPQRSTUVWXYZ    Date Introduced       Model Name & Number
 * --------------------------------------------------------------------------------------------
 * Beta                LEYJVCNIXWPBQMDRTAKZGFUHOS    Spring 1941           M4 R2
 * Gamma               FSOKANUERHMBTIYCWLQPZXVGJD    Spring 1942           M4 R2
 * Reflector A         EJMZALYXVBWFCRQUONTSPIKHGD
 * Reflector B         YRUHQSLDPXNGOKMIEBFZCWVJAT
 * Reflector C         FVPJIAOYEDRZXWGCTKUQSBNMHL
 * Reflector B Thin    ENKQAUYWJICOPBLMDXZVFTHRGS    1940                  M4 R1 (M3 + Thin)
 * Reflector C Thin    RDOBJNTKVEHMLFCWZAXGYIPSUQ    1940                  M4 R1 (M3 + Thin)
 * --------------------------------------------------------------------------------------------
 * ETW                 ABCDEFGHIJKLMNOPQRSTUVWXYZ                          Enigma I
 * --------------------------------------------------------------------------------------------
 *
 */

/* Rotor wirings */
/* Encryption parameters follow */
typedef struct P
{
    const unsigned char rotor[5][26];
    const unsigned char ref1[26];
    const unsigned char notch1[5];
    char order[5];/*={ 1, 2, 3, 4, 5 };*/
    char rings[5];/*={ 'A','A','A','A','A'};*/
    char pos[5];/*={ 'A','A','A','A','A'};*/
    char cyph[MSGLEN];
    char crib[MSGLEN];
    char plug[10];/*=\"ASKINGLOPE\";*/
} Params;

#endif /* add_h */
