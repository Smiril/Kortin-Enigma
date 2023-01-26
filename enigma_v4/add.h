//
//  add.h
//  enigma
//
//  Created by Smiril on 20.07.22.
//  Copyright © 2022 Smiril. All rights reserved.
//

#ifndef add_h
#define add_h

#include "addx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>
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
#include <iconv.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <signal.h>
// on troubles openssl see: https://stackoverflow.com/questions/69002453/how-to-build-openssl-for-m1-and-for-intel
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/tls1.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define SERVICE "https" // possible http https
#define PROTO "tcp" // possible tcp udp icmp igmp raw
#define USERAGENT "Enigma/0.0.4 (iPad;CPU OS 13_1_2 like Mac OS X) AppleWebKit/602.2.14 (KHTML, like Gecko) Version/13.0 Mobile/14B100 SafaRI/602."
#define DELIM "."
#define BUFSIZE 1400
#define BUFSIZ2 4096
#define HOME "/usr/local/share/enigma/"
#define CERTF HOME "valid-root-ca.pem"
#define KEYF HOME "valid-root-cakey.pem"

int flag;
int tempres = BUFSIZ2;
int err;
int sd;
int sock2;
struct sockaddr_in sa_serv;
struct sockaddr_in sa;
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

#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) {perror(s);exit (1);}
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit (2); }

struct sockaddr_in *remote;
int tmpresx;
unsigned long int tmpres;
char *ip;
char *ipp;
char *ipprox;
char *get;
char buf[BUFSIZ+1];
char host[MSGY];
char page[MSGY];
char port[MSGY];
char proxy[MSGY];
char proxyport[MSGY];

pthread_mutex_t lock;

char *x = "A";
char *s = "A";
char *fff = "A";
char nerd[MSGX];
char framex[MSGY];
char flames[MSGY];
char *ukw;
char *nox;
int oldrank = IN_RANK;


/* Encryption parameters follow */
typedef struct main_ctx
{
        /* Rotor wirings */
        char rotor[MSGG][MSGC];
        char ref1[MSGC];
        char notch1[MSGG];
        char order[MSGG];/*={ 1, 3, 5, 7, 9 };*/
        char rings[MSGG];/*={ 'A','A','A','A','A'};*/
        char pos[MSGG];/*={ 'A','A','A','A','A'};*/
        char cyph[MSGLEN];
        char crib[MSGLEN];
        char plug[MSGP];/*=\"ASKINGLOPE\";*/
} main_ctx_t;

#endif /* add_h */
