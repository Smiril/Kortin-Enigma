//
//  add.h
//  enigma
//
//  Created by Smiril on 29.01.23.
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
#include <libxml/tree.h>
#include <signal.h>
// on troubles openssl see: https://stackoverflow.com/questions/69002453/how-to-build-openssl-for-m1-and-for-intel
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/tls1.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

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
char port[MSGY];
char proxy[MSGY];
char proxyport[MSGY];

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

char *x = "A";
char *s = "A";
char *fff = "A";
char nerd[MSGX];
char chad[MSGX];
char framex[MSGY];
char flames[MSGY];
char *ukw;
char *nox;
int oldrank = IN_RANK;
int fds[2];

/* Encryption parameters follow */
typedef struct main_ctx
{
        /* Rotor wirings */
        char    rotor[5][MSGC];
        char    ref1[MSGC];
        char    notch1[MSGG];
        int     order[MSGG];/*={ 1, 3, 5, 7, 9 };*/
        char    rings[MSGG];/*={ 'A','A','A','A','A'};*/
        char    pos[MSGG];/*={ 'A','A','A','A','A'};*/
        char    cyph[MSGLEN];
        char    crib[MSGLEN];
        char    plug[MSGP];/*=\"ASKINGLOPE\";*/
} main_ctx_t;

#endif /* add_h */
