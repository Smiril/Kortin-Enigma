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
******************************************************************************/
#define CA_PRIVATE_IMPLEMENTATION
#define VK_RETURN '\n'

#include "add.h"

#include "qgr.h"

extern float qgram[];

char *getfirstpassword();

void savestatus() {
    xmlNodePtr root = NULL;
    xmlNodePtr node = NULL;
    xmlChar* tmp = NULL;
    if ((strlen(statname) > 0) && status) {
        root = xmlDocGetRootElement(status);
        if (root) {
            xmlMutexLock(finishedMutex);
            for (node = root->children; node; node = node->next) {
                if (xmlStrcmp(node->name, (const xmlChar*)"current") == 0) {
                    xmlMutexLock(pwdMutex);
                    tmp = xmlEncodeEntitiesReentrant(status, (const xmlChar*) &password);
                    xmlMutexUnlock(pwdMutex);

                    if (node->children) {
                        if (password[0] == '\0') {
                            xmlNodeSetContent(node->children, (const xmlChar*)getfirstpassword());
                        } else {
                            xmlNodeSetContent(node->children, tmp);
                        }
                    }

                    xmlFree(tmp);
                } else if ((finished == 1) && (xmlStrcmp(node->name, (const xmlChar*)"good_password") == 0)) {
                    tmp =  xmlEncodeEntitiesReentrant(status, (const xmlChar*) &password_good);

                    if (node->children) {
                        xmlNodeSetContent(node->children, tmp);
                    }

                    xmlFree(tmp);
                }
            }
            xmlMutexUnlock(finishedMutex);
        }
        xmlSaveFormatFileEnc(statname, status, "UTF-8", 1);
    }
}

int loadstatus() {
    xmlNodePtr root = NULL;
    xmlNodePtr node = NULL;
    xmlParserCtxtPtr parserctxt;
    int ret = 0;
    char* tmp;
    FILE* totest;
    totest = fopen(statname, "r");
    if (totest) {
        fclose(totest);
        status = xmlParseFile(statname);
    }

    if (status) {
        root = xmlDocGetRootElement(status);
    } else {
        status = xmlNewDoc(NULL);
    }

    if (root) {
        parserctxt = xmlNewParserCtxt();
        for (node = root->children; node; node = node->next) {
            if (xmlStrcmp(node->name, (const xmlChar*)"abc") == 0) {
                if (node->children && (strlen((const char*)node->children->content) > 0)) {
                    ABC = (char *)xmlStringDecodeEntities(parserctxt, (const xmlChar*)node->children->content, XML_SUBSTITUTE_BOTH, 0, 0, 0);
                } else {
                    ret = 1;
                }
            } else if (xmlStrcmp(node->name, (const xmlChar*)"current") == 0) {
                if (node->children && (strlen((const char*)node->children->content) > 0)) {
                    tmp = (char *)xmlStringDecodeEntities(parserctxt, (const xmlChar*)node->children->content, XML_SUBSTITUTE_BOTH, 0, 0, 0);
                    strcpy(password,tmp);
                    curr_len = strlen(password);
                    printf("INFO: Resuming cracking from Message: '%s'\n",password);
                    xmlFree(tmp);
                } else {
                    ret = 1;
                }
            } else if (xmlStrcmp(node->name, (const xmlChar*)"good_password") == 0) {
                if (node->children && (strlen((const char*)node->children->content) > 0)) {
                    tmp = (char *)xmlStringDecodeEntities(parserctxt, node->children->content, XML_SUBSTITUTE_BOTH,0,0,0);
                    strcpy(password,tmp);
                    curr_len = strlen(password);
                    xmlMutexLock(finishedMutex);
                    finished = 1;
                    xmlMutexUnlock(finishedMutex);
                    strcpy((char*) &password_good, (char*) &password);
                    printf("GOOD: This archive was succesfully cracked\n");
                    printf("      The good password is: '%s'\n", password);
                    xmlFree(tmp);
                    ret = 1;
                }
            }
        }

        xmlFreeParserCtxt(parserctxt);
    } else {
        root = xmlNewNode(NULL, (const xmlChar*)"rarcrack");
        xmlDocSetRootElement(status, root);
        node = xmlNewTextChild(root, NULL, (const xmlChar*)"abc", (const xmlChar*)ABC);
        node = xmlNewTextChild(root, NULL, (const xmlChar*)"current", (const xmlChar*)getfirstpassword());
        node = xmlNewTextChild(root, NULL, (const xmlChar*)"good_password", (const xmlChar*)"");
        savestatus();
    }

    return ret;
}

char *getfirstpassword() {
    static char ret[2];
    ret[0] = ABC[0];
    ret[1] = '\0';
    return (char*) &ret;
}

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
    
    //fclose(fp);
    
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
                //printf("%s\n",uri);
                //printf("%s\n",main_ctx->rotor[count1]);
                //xmlFree(uri);
            }
        }
        else if (xmlStrcmp(gchild->name, (const xmlChar *)configx[count2++])) {
            if((uri = xmlGetProp(gchild,(const xmlChar *)"name")) != NULL) {
                strcpy((char *)p->ref1,(const char *)uri);
                //printf("%s\n",uri);
                //printf("%s\n",main_ctx->ref1[count2]);
                //xmlFree(uri);
            }
        }
        else if (xmlStrcmp(gchild->name, (const xmlChar *)configy[count3++])) {
            if((uri = xmlGetProp(gchild,(const xmlChar *)"name")) != NULL) {
                strcpy((char *)p->notch1,(const char *)uri);
                //printf("%s\n",uri);
                //printf("%s\n",main_ctx->notch1[count3]);
                //xmlFree(uri);
            }
        }
        gchild = gchild->next->next;
    }
    xmlFreeDoc(doc);
    fclose(fp);
}

//---------------------------------------------------------------------
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

static void m(void *array, size_t n, size_t size) {
    char tmp[size];
    char *arr = array;
    size_t stride = size * sizeof(char);

    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; ++i) {
            size_t rnd = (size_t) rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}

const char *felix(size_t fex)
{
    srand(time(NULL));
    char arr[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *picked = (char*) calloc(fex, sizeof(char*));
    
    m(arr, NELEMS(arr), sizeof(arr[0]));
    
    do {
    for (size_t i = 0; i < (fex); ++i) {
        picked[i] = arr[i];
        }
    } while (0);

    return picked;
}


#if defined(__WIN32__) && defined(__WIN64__)
int inet_pton(int af, const char *src, void *dst)
{
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN+1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
    switch(af) {
      case AF_INET:
    *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
        break;
      case AF_INET6:
    *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
        break;
    }
  }
  return 0;
}

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
  struct sockaddr_storage ss;
  unsigned long s = size;

  ZeroMemory(&ss, sizeof(ss));
  ss.ss_family = af;

  switch(af) {
    case AF_INET:
      ((struct sockaddr_in *)&ss)->sin_addr = *(struct in_addr *)src;
      break;
    case AF_INET6:
      ((struct sockaddr_in6 *)&ss)->sin6_addr = *(struct in6_addr *)src;
      break;
    default:
      return AF_INET;
  }
  /* cannot direclty use &size because of strict aliasing rules */
  return (WSAAddressToString((struct sockaddr *)&ss, sizeof(ss), NULL, dst & s) == 0)?dst : NULL;
}
#endif

static int ip_version(const char *ip) {
    struct addrinfo hint, *res = NULL;
    int ret;

    memset(&hint, 0, sizeof(hint));

    hint.ai_family = PF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;

    if ((ret = getaddrinfo(ip, NULL, &hint, &res)) == 0) {
        if(res->ai_family == AF_INET) {
            return 4;
        } else if (res->ai_family == AF_INET6) {
            return 6;
        } else {
            printf("%s is an is unknown address format %d\n",ip,res->ai_family);
            return ret;
        }
        freeaddrinfo(res);
    }
   
   return 0;
}

int valid_digit(char *ip_str)
{
    while (*ip_str) {
        if (*ip_str >= '0' && *ip_str <= '9')
            ++ip_str;
        else
            return 0;
    }
    return 1;
}

int is_valid_ip6(char *ip_str)
{
    int num, dots = 0;
    char *ptr;
 
    if (ip_str == NULL)
        return 0;
 
    ptr = strtok(ip_str, DELIM2);
 
    if (ptr == NULL)
        return 0;
 
    while (ptr) {
 
        num = atoi(ptr);
 
        /* check for valid IP */
        if(!(num < 0) && !(num > 65535)){
            ptr = strtok(NULL, DELIM2);
            if (ptr != NULL)
                ++dots;
        } else
            return 0;
    }
 
    /* valid IP string must contain 7 dots */
    if (dots != 7)
        return 0;
    return 1;
}

int is_valid_ip4(char *ip_str)
{
    int num, dots = 0;
    char *ptr;
 
    if (ip_str == NULL)
        return 0;
 
    ptr = strtok(ip_str, DELIM1);
 
    if (ptr == NULL)
        return 0;
 
    while (ptr) {
 
        /* after parsing string, it must contain only digits */
        if (!valid_digit(ptr))
            return 0;
 
        num = atoi(ptr);
 
        /* check for valid IP */
        if (num >= 0 && num <= 255) {
            /* parse remaining string */
            ptr = strtok(NULL, DELIM1);
            if (ptr != NULL)
                ++dots;
        } else
            return 0;
    }
 
    /* valid IP string must contain 3 dots */
    if (dots != 3)
        return 0;
    return 1;
}

int create_tcp_socket(void){
#if defined(__WIN32__) && defined(__WIN64__)
    WSADATA wsa;
    SOCKET socket_desc;
 
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d\n",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n]");
#else
  int socket_desc;
#endif
    
  if((socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    perror("Can't create TCP socket");
    exit(1);
  }
   else if((socket_desc = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0){
      perror("Can't create TCP socket");
      exit(1);
    }
    return socket_desc;
}

int create_udp_socket(void){
#if defined(__WIN32__) && defined(__WIN64__)
    WSADATA wsa;
    SOCKET socket_desc;
 
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d\n",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n]");
#else
  int socket_desc;
#endif

  if((socket_desc = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
    perror("Can't create UDP socket");
    exit(1);
  }
   else if((socket_desc = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0){
      perror("Can't create UDP socket");
      exit(1);
    }
    return socket_desc;
}

//note, to allow root to use icmp sockets, run:
//sysctl -w net.ipv4.ping_group_range="0 0"

int create_icmp_socket(void){
#if defined(__WIN32__) && defined(__WIN64__)
    WSADATA wsa;
    SOCKET socket_desc;
 
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d\n",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n]");
#else
  int socket_desc;
#endif

  if((socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP)) < 0){
    perror("Can't create ICMP socket");
    exit(1);
  }
   else if((socket_desc = socket(AF_INET6, SOCK_DGRAM, IPPROTO_ICMP)) < 0){
      perror("Can't create ICMP socket");
      exit(1);
    }
    return socket_desc;
}

int create_igmp_socket(void){
#if defined(__WIN32__) && defined(__WIN64__)
    WSADATA wsa;
    SOCKET socket_desc;
 
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d\n",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n]");
#else
  int socket_desc;
#endif

  if((socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0){
    perror("Can't create IGMP socket");
    exit(1);
  }
   else if((socket_desc = socket(AF_INET6, SOCK_DGRAM, IPPROTO_IP)) < 0){
      perror("Can't create IGMP socket");
      exit(1);
    }
    return socket_desc;
}

int create_raw_socket(void){
#if defined(__WIN32__) && defined(__WIN64__)
    WSADATA wsa;
    SOCKET socket_desc;
 
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d\n",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n]");
#else
  int socket_desc;
#endif

    if((socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_RAW)) < 0){
    perror("Can't create RAW socket");
    exit(1);
    }
    else if((socket_desc = socket(AF_INET6, SOCK_STREAM, IPPROTO_RAW)) < 0){
      perror("Can't create RAW socket");
      exit(1);
    }
  return socket_desc;
}

char *get_ip(char *host){
    struct hostent *hent;
    int iplen = 16;
    char *ipx = (char *)malloc(iplen);
    memset(ipx,0,iplen);
    
    if((hent = gethostbyname(host)) == NULL){
        printf("Error: %d\n",errno);
        perror("can not get ip");
        exit(1);
    }

    if((inet_ntop(AF_INET,(void *)hent->h_addr_list,ipx,iplen)) == 0){
        perror("getting ipv4 host");
    }
    else if((inet_ntop(AF_INET6,(void *)hent->h_addr_list,ipx,iplen)) == 0){
        perror("getting ipv6 host");
    }
    
    return ipx;
}

char *d_build_get_query_igmp(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-3);
    sprintf(query,tpl,getpage,host,USERAGENT);
            
    return query;
}

char *d_build_get_query_icmp(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-3);
    sprintf(query,tpl,getpage,host,USERAGENT);
            
    return query;
}

char *d_build_get_query_tcp(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-3);
    sprintf(query,tpl,getpage,host,USERAGENT);
            
    return query;
}

char *d_build_get_query_udp(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-3);
    sprintf(query,tpl,getpage,host,USERAGENT);
            
    return query;
}

char *d_build_get_query_raw(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-3);
    sprintf(query,tpl,getpage,host,USERAGENT);
            
    return query;
}

char *d_build_getcl(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: close:\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-3);
    sprintf(query,tpl,getpage,host,USERAGENT);
            
    return query;
}

char *build_get_query_igmp(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(proxy)+strlen((const char *)proxyport)+strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-6);
    sprintf(query,tpl,proxy,proxyport,getpage,host,USERAGENT);
            
    return query;
}

char *build_get_query_icmp(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(proxy)+strlen((const char *)proxyport)+strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-6);
    sprintf(query,tpl,proxy,proxyport,getpage,host,USERAGENT);
            
    return query;
}

char *build_get_query_tcp(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(proxy)+strlen((const char *)proxyport)+strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-6);
    sprintf(query,tpl,proxy,proxyport,getpage,host,USERAGENT);
            
    return query;
}

char *build_get_query_udp(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(proxy)+strlen((const char *)proxyport)+strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-6);
    sprintf(query,tpl,proxy,proxyport,getpage,host,USERAGENT);
            
    return query;
}

char *build_get_query_raw(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: keep-alive\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(proxy)+strlen((const char *)proxyport)+strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-6);
    sprintf(query,tpl,proxy,proxyport,getpage,host,USERAGENT);
            
    return query;
}

char *build_getcl(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: text/xml\r\nConnection: close:\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(proxy)+strlen((const char *)proxyport)+strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-6);
    sprintf(query,tpl,proxy,proxyport,getpage,host,USERAGENT);
            
    return query;
}

void *connection_handler_d(Params *main_ctx,char *host,char *port,char *page){
    if( strcmp (SERVICE,"https") == 0){
        SSL_load_error_strings();
        OpenSSL_add_ssl_algorithms();
        meth = SSLv23_method();
        ctx = SSL_CTX_new(meth);
        CHK_NULL(ctx);
        
        if(!ctx){
            printf("Theres no Crypto Method choosen!\n");
            exit(1);
        }
        
        if(SSL_CTX_load_verify_locations(ctx,CERTF,HOME) < 0){
            printf("Verify of the cert failed!\n");
            exit(1);
        }
        
        if(SSL_CTX_use_certificate_file(ctx,CERTF,SSL_FILETYPE_PEM) < 0){
            printf("certfile is not valid\n");
            exit(1);
        }
        
        if(SSL_CTX_use_PrivateKey_file(ctx,KEYF,SSL_FILETYPE_PEM) < 0){
            printf("keyfile is not valid\n");
            exit(1);
        }

        if(SSL_CTX_check_private_key(ctx) < 0){
            printf("Private key does not match the Public key\n");
            exit(1);
        }
    }
    
    sock2 = 1;
    
    if( strcmp (PROTO,"tcp") == 0){
    sock2 = create_tcp_socket();
    }
    else if( strcmp (PROTO,"udp") == 0){
    sock2 = create_udp_socket();
    }
    else if( strcmp (PROTO,"icmp") == 0){
    sock2 = create_icmp_socket();
    }
    else if( strcmp (PROTO,"igmp") == 0){
    sock2 = create_igmp_socket();
    }
    else if( strcmp (PROTO,"raw") == 0){
    sock2 = create_raw_socket();
    }
    else {
        perror("Wrong Protocol");
        exit(1);
    }
    
    ip = get_ip(host);
    if(6 == ip_version(ip)) {
#if !defined(__WIN32__) && !defined(__WIN64__)
    is_valid_ip6(ip)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
    is_valid_ip6(ip)? printf("Valid\n"): printf("Not valid\n");
#endif

    printf("connect to %s \n",ip);
    sa6.sin6_family = AF_INET6;
    sa6.sin6_port = htons(rand() % 1000 + 20000);
    sa6.sin6_addr = in6addr_any;
    
    remote6 = (struct sockaddr_in6 *)malloc(sizeof(struct sockaddr_in6 *));
    remote6->sin6_family = AF_INET6;
    remote6->sin6_port = htons(atoi(port));
    tmpresx = inet_pton(AF_INET6, ip, (void *)(&(remote6->sin6_addr.s6_addr)));
    }
    else if(4 == ip_version(ip)) {
#if !defined(__WIN32__) && !defined(__WIN64__)
    is_valid_ip4(ip)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
    is_valid_ip4(ip)? printf("Valid\n"): printf("Not valid\n");
#endif

    printf("connect to %s \n",ip);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(rand() % 1000 + 20000);
    sa.sin_addr.s_addr = INADDR_ANY;
    
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    remote->sin_port = htons(atoi(port));
    tmpresx = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
    }
    
    int res = 0;
    
    if(6 == ip_version(ip)) {
        if( ( res = connect(sock2,(struct sockaddr *)remote6,sizeof(struct sockaddr)) ) != 0){
            perror("could not connect");
            exit(1);
        }
    }
    else if(4 == ip_version(ip)) {
        if( ( res = connect(sock2,(struct sockaddr *)remote,sizeof(struct sockaddr)) ) != 0){
            perror("could not connect");
            exit(1);
        }
    }

    if (res == 0)
     {
    perror("connect");
       // OK -- socket is ready for IO
     }

    struct timeval timeout;
    timeout.tv_sec = 120;
    timeout.tv_usec = 0;

    if(setsockopt(sock2,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout)) < 0){
        perror("rcv sockopt failed");}

    if(setsockopt(sock2,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout)) < 0){
        perror("snd sockopt failed");}
    
    if( strcmp (SERVICE,"https") == 0){
        ssl = SSL_new(ctx);
        CHK_NULL(ssl);
        SSL_set_fd(ssl,sock2);
        err = SSL_connect(ssl);
        CHK_SSL(err);
        printf("SSL Connection using %s \n",SSL_get_cipher(ssl));
        server_cert = SSL_get_peer_certificate(ssl);
        CHK_NULL(server_cert);
        str = X509_NAME_oneline(X509_get_subject_name(server_cert),0,0);
        CHK_NULL(str);
        printf("\t subject: %s\n",str);
        OPENSSL_free(str);
        X509_free(server_cert);
    }
    
    if( strcmp (PROTO,"tcp") == 0){
    get = d_build_get_query_tcp(host,page);
    }
    else if( strcmp (PROTO,"udp") == 0){
    get = d_build_get_query_udp(host,page);
    }
    else if( strcmp (PROTO,"icmp") == 0){
    get = d_build_get_query_icmp(host,page);
    }
    else if( strcmp (PROTO,"igmp") == 0){
    get = d_build_get_query_igmp(host,page);
    }
    else if( strcmp (PROTO,"raw") == 0){
    get = d_build_get_query_raw(host,page);
    }
    else {
        perror("Wrong Protocol");
        exit(1);
    }

    if( strcmp (PROTO,"tcp") == 0){
    get_cl = d_build_getcl(host,page);
    }
    else if( strcmp (PROTO,"udp") == 0){
    get_cl = d_build_getcl(host,page);
    }
    else if( strcmp (PROTO,"icmp") == 0){
    get_cl = d_build_getcl(host,page);
    }
    else if( strcmp (PROTO,"igmp") == 0){
    get_cl = d_build_getcl(host,page);
    }
    else if( strcmp (PROTO,"raw") == 0){
    get_cl = d_build_getcl(host,page);
    }
    else {
        perror("Wrong Protocol");
        exit(1);
    }
    
#if !defined(__WIN32__) && !defined(__WIN64__)
    fprintf(stderr,"Query is:\n\x1B[32m<<START>>\x1B[39m\n\x1B[33m%s\x1B[39m\n\x1B[32m<<END>>\x1B[39m\n",get);
#elif !defined(__APPLE__) && !defined(__LINUX__)
    fprintf(stderr,"Query is:\n<<START>>\n%s\n<<END>>\n",get);
#endif
    
    int sent = 0;
    
    if( strcmp (SERVICE,"https") == 0){

            tmpres = SSL_write(ssl,get+sent,strlen(get)-sent);
            if(tmpres != 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    if( strcmp (SERVICE,"http") == 0){
        
            tmpres = send(sock2,get+sent,strlen(get)-sent,0);
            if(tmpres != 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    memset(buf,0,strlen(buf));
    
    int htmlstart = 0;
    char *htmlcontent;
    FILE *rp;

    if( strcmp (SERVICE,"https") == 0){
        while((tmpres = SSL_read(ssl,buf,BUFSIZ)) == 0){
            if(htmlstart == 0){
                htmlcontent = strstr(buf,"\r\n\r\n");
                htmlstart = 1;
                //htmlcontent += 4;
            }else{
                htmlcontent = buf ;
            }

            memset(buf,0,strlen(htmlcontent));
            
            if(htmlcontent == 0){
                perror("Error reciving data");
            }
            else {
                
#if !defined(__WIN32__) && !defined(__WIN64__)
                rp = fopen("/usr/local/share/enigma/remote.xml","w");
#endif
#if !defined(__APPLE__) && !defined(__LINUX__)
                rp = fopen("C:\\usr\\local\\share\\enigma\\remote.xml","w");
#endif
                fprintf(rp,"%s",htmlcontent);
                fclose(rp);

#if !defined(__WIN32__) && !defined(__WIN64__)
                configmain(main_ctx,"/usr/local/share/enigma/remote.xml");
#endif
#if !defined(__APPLE__) && !defined(__LINUX__)
                configmain(main_ctx,"C:\\usr\\local\\share\\enigma\\remote.xml");
#endif
                break;
            }
        }
    }
    
    if( strcmp (SERVICE,"http") == 0){
        while((tmpres = recv(sock2,buf,BUFSIZ,0)) == 0){
            if(htmlstart == 0){
                htmlcontent = strstr(buf,"\r\n\r\n");
                htmlstart = 1;
                //htmlcontent += 4;
            }else{
                htmlcontent = buf ;
            }
            
            memset(buf,0,strlen(htmlcontent));
        
            if(htmlcontent == 0){
                perror("error reciving data");
            }
            else {
                
#if !defined(__WIN32__) && !defined(__WIN64__)
                rp = fopen("/usr/local/share/enigma/remote.xml","w");
#endif
#if !defined(__APPLE__) && !defined(__LINUX__)
                rp = fopen("C:\\usr\\local\\share\\enigma\\remote.xml","w");
#endif
                fprintf(rp,"%s",htmlcontent);
                fclose(rp);

#if !defined(__WIN32__) && !defined(__WIN64__)
                configmain(main_ctx,"/usr/local/share/enigma/remote.xml");
#endif
#if !defined(__APPLE__) && !defined(__LINUX__)
                configmain(main_ctx,"C:\\usr\\local\\share\\enigma\\remote.xml");
#endif

                break;
            }
        }
    }

    if( strcmp (SERVICE,"https") == 0){

            tmpres = SSL_write(ssl,get_cl+sent,strlen(get_cl)-sent);
            if(tmpres != 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    if( strcmp (SERVICE,"http") == 0){
        
            tmpres = send(sock2,get_cl+sent,strlen(get_cl)-sent,0);
            if(tmpres != 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    if( strcmp (SERVICE,"https") == 0){
        SSL_shutdown(ssl);
    }
        close(sock2);
    
    if( strcmp (SERVICE,"https") == 0){
        SSL_free(ssl);
        SSL_CTX_free(ctx);
    }

    return 0;
}

void *connection_handler(Params *main_ctx,char *proxy,char *proxyport,char *host,char *page){
    if( strcmp (SERVICE,"https") == 0){
        SSL_load_error_strings();
        OpenSSL_add_ssl_algorithms();
        meth = SSLv23_method();
        ctx = SSL_CTX_new(meth);
        CHK_NULL(ctx);
        
        if(!ctx){
            printf("Theres no Crypto Method choosen!\n");
            exit(1);
        }
        
        if(SSL_CTX_load_verify_locations(ctx,CERTF,HOME) < 0){
            printf("Verify of the cert failed!\n");
            exit(1);
        }
        
        if(SSL_CTX_use_certificate_file(ctx,CERTF,SSL_FILETYPE_PEM) < 0){
            printf("certfile is not valid\n");
            exit(1);
        }
        
        if(SSL_CTX_use_PrivateKey_file(ctx,KEYF,SSL_FILETYPE_PEM) < 0){
            printf("keyfile is not valid\n");
            exit(1);
        }

        if(SSL_CTX_check_private_key(ctx) < 0){
            printf("Private key does not match the Public key\n");
            exit(1);
        }
    }
    
    sock2 = 1;
    
    if( strcmp (PROTO,"tcp") == 0){
    sock2 = create_tcp_socket();
    }
    else if( strcmp (PROTO,"udp") == 0){
    sock2 = create_udp_socket();
    }
    else if( strcmp (PROTO,"icmp") == 0){
    sock2 = create_icmp_socket();
    }
    else if( strcmp (PROTO,"igmp") == 0){
    sock2 = create_igmp_socket();
    }
    else if( strcmp (PROTO,"raw") == 0){
    sock2 = create_raw_socket();
    }
    else {
        perror("Wrong Protocol");
        exit(1);
    }
    
    ip = get_ip(host);
    ipp = get_ip(proxy);
    if(6 == ip_version(ipp)) {
        printf("go from %s ",ipp);
#if !defined(__WIN32__) && !defined(__WIN64__)
        is_valid_ip6(ipp)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
        is_valid_ip6(ipp)? printf("Valid\n"): printf("Not valid\n");
#endif
        printf("connect to %s \n",ip);
        sa6.sin6_family = AF_INET6;
        sa6.sin6_port = htons(rand() % 1000 + 20000);
        sa6.sin6_addr = in6addr_any;
        
        remote6 = (struct sockaddr_in6 *)malloc(sizeof(struct sockaddr_in6 *));
        remote6->sin6_family = AF_INET6;
        remote6->sin6_port = htons(atoi(proxyport));
        tmpresx = inet_pton(AF_INET6, ipp, (void *)(&(remote6->sin6_addr.s6_addr)));
        
#if !defined(__WIN32__) && !defined(__WIN64__)
        is_valid_ip6(ip)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
        is_valid_ip6(ip)? printf("Valid\n"): printf("Not valid\n");
#endif
    }
    else if(4 == ip_version(ipp)) {
        printf("go from %s ",ipp);
#if !defined(__WIN32__) && !defined(__WIN64__)
        is_valid_ip4(ipp)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
        is_valid_ip4(ipp)? printf("Valid\n"): printf("Not valid\n");
#endif
        printf("connect to %s \n",ip);
        sa.sin_family = AF_INET;
        sa.sin_port = htons(rand() % 1000 + 20000);
        sa.sin_addr.s_addr = INADDR_ANY;
        
        remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
        remote->sin_family = AF_INET;
        remote->sin_port = htons(atoi(proxyport));
        tmpresx = inet_pton(AF_INET, ipp, (void *)(&(remote->sin_addr.s_addr)));
        
#if !defined(__WIN32__) && !defined(__WIN64__)
        is_valid_ip4(ip)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
        is_valid_ip4(ip)? printf("Valid\n"): printf("Not valid\n");
#endif
    }
    
    int res = 0;
    
    if(6 == ip_version(ip)) {
        if( ( res = connect(sock2,(struct sockaddr *)remote6,sizeof(struct sockaddr)) ) != 0){
            perror("could not connect");
            exit(1);
        }
    }
    else if(4 == ip_version(ip)) {
        if( ( res = connect(sock2,(struct sockaddr *)remote,sizeof(struct sockaddr)) ) != 0){
            perror("could not connect");
            exit(1);
        }
    }

    if (res == 0)
     {
    perror("connect");
       // OK -- socket is ready for IO
     }

    struct timeval timeout;
    timeout.tv_sec = 120;
    timeout.tv_usec = 0;

    if(setsockopt(sock2,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout)) < 0){
        perror("rcv sockopt failed");}

    if(setsockopt(sock2,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout)) < 0){
        perror("snd sockopt failed");}
    
    if( strcmp (SERVICE,"https") == 0){
        ssl = SSL_new(ctx);
        CHK_NULL(ssl);
        SSL_set_fd(ssl,sock2);
        err = SSL_connect(ssl);
        CHK_SSL(err);
        printf("SSL Connection using %s \n",SSL_get_cipher(ssl));
        server_cert = SSL_get_peer_certificate(ssl);
        CHK_NULL(server_cert);
        str = X509_NAME_oneline(X509_get_subject_name(server_cert),0,0);
        CHK_NULL(str);
        printf("\t subject: %s\n",str);
        OPENSSL_free(str);
        X509_free(server_cert);
    }
    
    if( strcmp (PROTO,"tcp") == 0){
    get = build_get_query_tcp(host,page);
    }
    else if( strcmp (PROTO,"udp") == 0){
    get = build_get_query_udp(host,page);
    }
    else if( strcmp (PROTO,"icmp") == 0){
    get = build_get_query_icmp(host,page);
    }
    else if( strcmp (PROTO,"igmp") == 0){
    get = build_get_query_igmp(host,page);
    }
    else if( strcmp (PROTO,"raw") == 0){
    get = build_get_query_raw(host,page);
    }
    else {
        perror("Wrong Protocol");
        exit(1);
    }
    
    if( strcmp (PROTO,"tcp") == 0){
    get_cl = build_getcl(host,page);
    }
    else if( strcmp (PROTO,"udp") == 0){
    get_cl = build_getcl(host,page);
    }
    else if( strcmp (PROTO,"icmp") == 0){
    get_cl = build_getcl(host,page);
    }
    else if( strcmp (PROTO,"igmp") == 0){
    get_cl = build_getcl(host,page);
    }
    else if( strcmp (PROTO,"raw") == 0){
    get_cl = build_getcl(host,page);
    }
    else {
        perror("Wrong Protocol");
        exit(1);
    }
    
#if !defined(__WIN32__) && !defined(__WIN64__)
    fprintf(stderr,"Query is:\n\x1B[32m<<START>>\x1B[39m\n\x1B[33m%s\x1B[39m\n\x1B[32m<<END>>\x1B[39m\n",get);
#elif !defined(__APPLE__) && !defined(__LINUX__)
    fprintf(stderr,"Query is:\n<<START>>\n%s\n<<END>>\n",get);
#endif
    
    int sent = 0;
    
    if( strcmp (SERVICE,"https") == 0){

            tmpres = SSL_write(ssl,get+sent,strlen(get)-sent);
            if(tmpres != 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    if( strcmp (SERVICE,"http") == 0){
        
            tmpres = send(sock2,get+sent,strlen(get)-sent,0);
            if(tmpres != 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    memset(buf,0,strlen(buf));
    
    int htmlstart = 0;
    char *htmlcontent;
    FILE *rp;

    if( strcmp (SERVICE,"https") == 0){
        while((tmpres = SSL_read(ssl,buf,BUFSIZ)) == 0){
            if(htmlstart == 0){
                htmlcontent = strstr(buf,"\r\n\r\n");
                htmlstart = 1;
                //htmlcontent += 4;
            }else{
                htmlcontent = buf ;
            }
            
            memset(buf,0,strlen(htmlcontent));
            
            if(htmlcontent == 0){
                perror("Error reciving data");
            }
            else {
            
#if !defined(__WIN32__) && !defined(__WIN64__)
                rp = fopen("/usr/local/share/enigma/remote.xml","w");
#endif
#if !defined(__APPLE__) && !defined(__LINUX__)
                rp = fopen("C:\\usr\\local\\share\\enigma\\remote.xml","w");
#endif
                fprintf(rp,"%s",htmlcontent);
                fclose(rp);

#if !defined(__WIN32__) && !defined(__WIN64__)
                configmain(main_ctx,"/usr/local/share/enigma/remote.xml");
#endif
#if !defined(__APPLE__) && !defined(__LINUX__)
                configmain(main_ctx,"C:\\usr\\local\\share\\enigma\\remote.xml");
#endif
                break;
            }
        }
    }
    
    if( strcmp (SERVICE,"http") == 0){
        while((tmpres = recv(sock2,buf,BUFSIZ,0)) == 0){
            if(htmlstart == 0){
                htmlcontent = strstr(buf,"\r\n\r\n");
                htmlstart = 1;
                //htmlcontent += 4;
            }else{
                htmlcontent = buf ;
            }
            
            memset(buf,0,strlen(htmlcontent));
        
            if(htmlcontent == 0){
                perror("error reciving data");
            }
            else {
                
#if !defined(__WIN32__) && !defined(__WIN64__)
                rp = fopen("/usr/local/share/enigma/remote.xml","w");
#endif
#if !defined(__APPLE__) && !defined(__LINUX__)
                rp = fopen("C:\\usr\\local\\share\\enigma\\remote.xml","w");
#endif
                fprintf(rp,"%s",htmlcontent);
                fclose(rp);

#if !defined(__WIN32__) && !defined(__WIN64__)
                configmain(main_ctx,"/usr/local/share/enigma/remote.xml");
#endif
#if !defined(__APPLE__) && !defined(__LINUX__)
                configmain(main_ctx,"C:\\usr\\local\\share\\enigma\\remote.xml");
#endif
                break;
            }
        }
    }
    
    if( strcmp (SERVICE,"https") == 0){

            tmpres = SSL_write(ssl,get_cl+sent,strlen(get_cl)-sent);
            if(tmpres != 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    if( strcmp (SERVICE,"http") == 0){
        
            tmpres = send(sock2,get_cl+sent,strlen(get_cl)-sent,0);
            if(tmpres != 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    if( strcmp (SERVICE,"https") == 0){
        SSL_shutdown(ssl);
    }
        close(sock2);
    
    if( strcmp (SERVICE,"https") == 0){
        SSL_free(ssl);
        SSL_CTX_free(ctx);
    }

    return 0;
}

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

const char* Versionx(void) {
#ifdef VERSION
    return VERSION;
#else
  return "WarGames 0.5 T.E.D. - The Enemy Dail - ENEMY-MODE";
#endif
}

int getRank(char *cyph) {

    char str[40];
    int  rank = 0;

    FILE * dict = fopen(framex, "r");
        if(!dict) return 0; /* cant open file */
    
    while(fgets(str,sizeof(str),dict) != NULL) {
        str[strlen(str)-1] = '\0';
        if(strstr(cyph, str) != NULL) rank++;
        rank = -scoreTextQgram(str,strlen(str));
    }

    fclose(dict);
    return(rank);
}

/*take a char and return its encoded version according to the
  encryption params, update params, i.e. advance wheels
  this part uses Fauzan Mirza's code*/
char scramble(char c, Params *p)
{
    unsigned long int flag = 0;
    int j,i;
        c=toupper(c);
        if (!isalpha(c))
            return -1;
        if (isspace(c))
            c = '-';
 
    for(unsigned long int f = 0;f <= strlen(&c);f++) {
        /* Step up first rotor */
        p->pos[0]++;
        if (p->pos[0]>'Z')
            p->pos[0] -= 26;
        /*  Step up second rotor if first rotor reached notch */
        if (p->pos[0]==nox[p->order[0]-1])
        {
            p->pos[1]++;
            if (p->pos[1]>'Z')
                p->pos[1] -= 26;
            if (p->pos[0]==nox[p->order[1]-1])
            {
                p->pos[1]++;
                if (p->pos[1]>'Z')
                    p->pos[1] -= 26;
                if (p->pos[0]==nox[p->order[2]-1])
                {
                    p->pos[1]++;
                    if (p->pos[1]>'Z')
                      p->pos[1] -= 26;
                    if (p->pos[0]==nox[p->order[3]-1])
                    {
                        p->pos[1]++;
                        if (p->pos[1]>'Z')
                            p->pos[1] -= 26;
                        if (p->pos[0]==nox[p->order[4]-1])
                        {
                            p->pos[1]++;
                            if (p->pos[1]>'Z')
                                p->pos[1] -= 26;
                            flag=f;
            /* Set flag if second rotor reached notch */
                            if (p->pos[1]==nox[p->order[0]-1])
                            {
                                p->pos[2]++;
                                if (p->pos[2]>'Z')
                                    p->pos[2] -= 26;
                                if (p->pos[1]==nox[p->order[1]-1])
                                {
                                    p->pos[2]++;
                                    if (p->pos[2]>'Z')
                                        p->pos[2] -= 26;
                                    if (p->pos[1]==nox[p->order[2]-1])
                                    {
                                        p->pos[2]++;
                                        if (p->pos[2]>'Z')
                                            p->pos[2] -= 26;
                                        if (p->pos[1]==nox[p->order[3]-1])
                                        {
                                            p->pos[2]++;
                                            if (p->pos[2]>'Z')
                                                p->pos[2] -= 26;
                                            if (p->pos[1]==nox[p->order[4]-1])
                                            {
                                                p->pos[2]++;
                                                if (p->pos[2]>'Z')
                                                    p->pos[2] -= 26;
                                                flag=f;
                                                if (p->pos[2]==nox[p->order[0]-1])
                                                {
                                                    p->pos[3]++;
                                                    if (p->pos[3]>'Z')
                                                        p->pos[3] -= 26;
                                                    if (p->pos[2]==nox[p->order[1]-1])
                                                    {
                                                        p->pos[3]++;
                                                        if (p->pos[3]>'Z')
                                                            p->pos[3] -= 26;
                                                        if (p->pos[2]==nox[p->order[2]-1])
                                                        {
                                                            p->pos[3]++;
                                                            if (p->pos[3]>'Z')
                                                                p->pos[3] -= 26;
                                                            if (p->pos[2]==nox[p->order[3]-1])
                                                            {
                                                                p->pos[3]++;
                                                                if (p->pos[3]>'Z')
                                                                    p->pos[3] -= 26;
                                                                if (p->pos[2]==nox[p->order[4]-1])
                                                                {
                                                                    p->pos[3]++;
                                                                    if (p->pos[3]>'Z')
                                                                        p->pos[3] -= 26;
                                                                    flag=f;
                                                                    if (p->pos[3]==nox[p->order[0]-1])
                                                                    {
                                                                        p->pos[4]++;
                                                                        if (p->pos[4]>'Z')
                                                                            p->pos[4] -= 26;
                                                                        if (p->pos[3]==nox[p->order[1]-1])
                                                                        {
                                                                            p->pos[4]++;
                                                                            if (p->pos[4]>'Z')
                                                                                p->pos[4] -= 26;
                                                                            if (p->pos[3]==nox[p->order[2]-1])
                                                                            {
                                                                                p->pos[4]++;
                                                                                if (p->pos[4]>'Z')
                                                                                    p->pos[4] -= 26;
                                                                                if (p->pos[3]==nox[p->order[3]-1])
                                                                                {
                                                                                    p->pos[4]++;
                                                                                    if (p->pos[4]>'Z')
                                                                                        p->pos[4] -= 26;
                                                                                    if (p->pos[3]==nox[p->order[4]-1])
                                                                                    {
                                                                                        p->pos[4]++;
                                                                                        if (p->pos[4]>'Z')
                                                                                            p->pos[4] -= 26;
                                                                                        flag=f;
                                                                                        if (p->pos[4]==nox[p->order[0]-1])
                                                                                        {
                                                                                            if (p->pos[4]==nox[p->order[1]-1])
                                                                                            {
                                                                                                if (p->pos[4]==nox[p->order[2]-1])
                                                                                                {
                                                                                                    if (p->pos[4]==nox[p->order[3]-1])
                                                                                                    {
                                                                                                        if (p->pos[4]==nox[p->order[4]-1])
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
            p->pos[0]++;
                if (p->pos[0]>'Z')
                    p->pos[0] -= 26;
            }
            if (flag > 1)
            {
                f++;
            p->pos[1]++;
                if (p->pos[1]>'Z')
                    p->pos[1] -= 26;
            }
            if (flag > 2)
            {
                f++;
            p->pos[2]++;
                if (p->pos[2]>'Z')
                    p->pos[2] -= 26;
            }
            if (flag > 3)
            {
                f++;
            p->pos[3]++;
                if (p->pos[3]>'Z')
                    p->pos[3] -= 26;
            }
            if (flag > 4)
            {
                f++;
            p->pos[4]++;
                if (p->pos[4]>'Z')
                    p->pos[4] -= 26;
                flag=0;
            }
        }
    }
    /*  Swap pairs of letters on the plugboard */
    for (unsigned long int k=0;p->plug[k]; k+=2)
    {
        if (c==p->plug[k])
            c=p->plug[k+1];
        else if (c==p->plug[k+1])
            c=p->plug[k];
    }

        /*  Rotors (forward) */
        for (i=0; i<5; i++)
        {
            c += p->pos[i]-'A';
            if (c>'Z')
                c -= 26;

            c -= p->rings[i]-'A';
            if (c<'A')
                c += 26;

            c=p->rotor[p->order[i]-1][c-'A'];

            c += p->rings[i]-'A';
            if (c>'Z')
                c -= 26;

            c -= p->pos[i]-'A';
            if (c<'A')
                c += 26;
        }

        /*  Reflecting rotor */

        c=ukw[c-'A'];
    
    /*  Rotors (reverse) */
        for (i=5; i; i--)
        {
            c += p->pos[i-1]-'A';
            if (c>'Z')
                c -= 26;

            c -= p->rings[i-1]-'A';
            if (c<'A')
                c += 26;

            for (j=0; j<26; j++)
                if (p->rotor[p->order[i-1]-1][j]==c)
                    break;
            c=j+'A';

            c += p->rings[i-1]-'A';
            if (c>'Z')
                c -= 26;

            c -= p->pos[i-1]-'A';
            if (c<'A')
                c += 26;
        }
        
    /*  Plugboard */
        for (unsigned long int k=0;p->plug[k]; k+=2)
        {
            if (c==p->plug[k])
                c=p->plug[k+1];
            else if (c==p->plug[k+1])
                c=p->plug[k];
        }

  return c;
}

/*take a string, return encoded string*/
char *enigma(char *in, Params *p)
{
    unsigned long int j;
    for(j = 0; j < strlen(in); j++)
    x[j] = scramble(in[j], p);
    x[j] = '\0';
    return x;
}

/*read in a string, and pass it through enigma*/
void cypher(Params p)
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
    strcpy(o, enigma(inx, &p));
    printf("%s\n%s\n", o, inx);
}

int rotate(int a, int b, int c, int d, int e, char *cyph, char *plug, int *ct,struct _opaque_pthread_t *tid)
{
    int rank = IN_RANK;
    Params p;
  p.order[0] = a;
  p.order[1] = b;
  p.order[2] = c;
  p.order[3] = d;
  p.order[4] = e;
    const time_t proc_start = time (NULL);
  //p.rings[0] = p.rings[1] = p.rings[2] = p.rings[3] = p.rings[4] = 'A';
  //p.pos[0] = p.pos[1] = p.pos[2] = p.pos[3] = p.pos[4] = 'A';
  strcpy(p.plug, plug);
  for(p.pos[0] = 'A'; p.pos[0] <= 'Z'; p.pos[0]++)
  {
    for(p.pos[1] = 'A'; p.pos[1] <= 'Z'; p.pos[1]++)
    {
        for(p.pos[2] = 'A'; p.pos[2] <= 'Z'; p.pos[2]++)
        {
            for(p.pos[3] = 'A'; p.pos[3] <= 'Z'; p.pos[3]++)
            {
                for(p.pos[4] = 'A'; p.pos[4] <= 'Z'; p.pos[4]++)
                {
                    for(p.rings[0] = 'A'; p.rings[0] <= 'Z'; p.rings[0]++)
                    {
                        for(p.rings[1] = 'A'; p.rings[1] <= 'Z'; p.rings[1]++)
                        {
                            for(p.rings[2] = 'A'; p.rings[2] <= 'Z'; p.rings[2]++)
                            {
                                for(p.rings[3] = 'A'; p.rings[3] <= 'Z'; p.rings[3]++)
                                {
                                    for(p.rings[4] = 'A'; p.rings[4] <= 'Z'; p.rings[4]++)
                                    {
                    
                                        Params cp = p;
                                        unsigned long int i = 0;
                                        int fail = 0;
                                        
                                        /* Calculate all cypher */
#if defined(__APPLE__)
                                        strlcpy(fff, enigma(cyph, &cp),sizeof(fff));
#elif defined(__LINUX__) || defined(__WIN32__) || defined(__WIN64__)
                                        strcpy(fff, enigma(cyph, &cp));
#endif
                                        
                                        rank = getRank(fff);

                                        while(strlen(fff) > i)
                                        {
                                            if(cyph[i] != scramble(fff[i], &cp)) fail++;
                                            
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
                                                           cp.order[0], cp.order[1], cp.order[2], cp.order[3], cp.order[4],
                                                           cp.pos[0], cp.pos[1], cp.pos[2], cp.pos[3], cp.pos[4],
                                                           cp.rings[0], cp.rings[1], cp.rings[2], cp.rings[3], cp.rings[4], cp.plug, fff, rank);
                                                    
                                                    oldrank = rank;
                                                }
                                                
                                            } else continue;
                                            
                                        if(strcmp(cyph, enigma(fff, &cp)) == 0)
                                        {
                                            printf("\x1B[33mWheels\x1B[39m \x1B[32m%d %d %d %d %d\x1B[39m \x1B[33mStart\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mRings\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mStecker\x1B[39m \"\x1B[32m%s\x1B[39m\"\nReflector: %s\nNOTCH: %s\n",
                                                   cp.order[0], cp.order[1], cp.order[2], cp.order[3], cp.order[4],
                                                   cp.pos[0], cp.pos[1], cp.pos[2], cp.pos[3], cp.pos[4],
                                                   cp.rings[0], cp.rings[1], cp.rings[2], cp.rings[3], cp.rings[4], cp.plug,ukw,nox);
                                            
                                            printf("%s decoded -> %s\n",cyph,enigma(cyph, &cp));
                                            const time_t proc_stop = time (NULL);
                                            printf("Time elapsed : begin %ld - end %ld \n",proc_start,proc_stop);
                                            pthread_join(tid, NULL);
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

void test(int a, int b, int c, int d, int e, char *cyph, int *ct,struct _opaque_pthread_t *tid) {
   
    printf("... calculating\n");
    const time_t proc_start = time (NULL);
    char A, B, C, D, E, F, G, H, I, J;
    int cs;
    
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
              int i = 0;
              s[i] = A;
              s[1] = B;
              s[2] = '\0';
              i++;
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
                                    
                                                              rotate(a, b, c, d, e, cyph, s, ct,tid);
                                                          }
                                                      }
                                                  }
                                                  else{
                                                      rotate(a, b, c, d, e, cyph, s, ct,tid);
                                                  }
                                              }
                                          }
                                      }
                                      else{
                                          rotate(a, b, c, d, e, cyph, s, ct,tid);
                                      }
                                  }
                              }
                          }
                          else{
                              rotate(a, b, c, d, e, cyph, s,  ct,tid);
                          }
                      }
                  }
              }
              else{
                  rotate(a, b, c, d, e, cyph, s, ct,tid);
                    }
                }
            }
        }
    }
    const time_t proc_stop = time (NULL);
    printf("Time elapsed Test: begin %ld - end %ld \n",proc_start,proc_stop);
}

/*run on all permutations of wheels a, b, c, d, e*/
void permute(int a, int b, int c, int d, int e, char *cyph, int *ct,struct _opaque_pthread_t *tid)
{
    printf("... testing\n");
    test(a, b, c, d, e, cyph, ct,tid);
    test(b, a, c, d, e, cyph, ct,tid);
    test(b, c, a, d, e, cyph, ct,tid);
    test(b, c, d, a, e, cyph, ct,tid);
    test(b, c, d, e, a, cyph, ct,tid);
    test(b, c, d, e, a, cyph, ct,tid);
    test(c, b, d, e, a, cyph, ct,tid);
    test(c, d, b, e, a, cyph, ct,tid);
    test(c, d, e, b, a, cyph, ct,tid);
    test(c, d, e, a, b, cyph, ct,tid);
    test(c, d, e, a, b, cyph, ct,tid);
    test(d, c, e, a, b, cyph, ct,tid);
    test(d, e, c, a, b, cyph, ct,tid);
    test(d, e, a, c, b, cyph, ct,tid);
    test(d, e, a, b, c, cyph, ct,tid);
    test(e, d, a, b, c, cyph, ct,tid);
    test(e, a, d, b, c, cyph, ct,tid);
    test(e, a, b, d, c, cyph, ct,tid);
    test(e, a, b, c, d, cyph, ct,tid);
    test(a, e, b, c, d, cyph, ct,tid);
    test(a, b, e, c, d, cyph, ct,tid);
    test(a, b, c, e, d, cyph, ct,tid);
}

void xinit(void)
{
    pthread_mutex_init(&lock, NULL);
}

void *status_thread() {
    int pwds;
    const short status_sleep = 3;
    while(1) {
        sleep(status_sleep);
        xmlMutexLock(finishedMutex);
        pwds = counter / status_sleep;
        counter = 0;

        if (finished != 0) {
            break;
        }

        xmlMutexUnlock(finishedMutex);
        xmlMutexLock(pwdMutex);
        printf("Probing: '%s' [%d pwds/sec]\n", password, pwds);
        xmlMutexUnlock(pwdMutex);
        savestatus();	//FIXME: this is wrong, when probing current password(s) is(are) not finished yet, and the program is exiting
    }
 }



/*all combinations of five possible wheels*/
int permuteAll(char *cyph,struct _opaque_pthread_t *tid)
{
    int ct = 0;
    for(int d = 1;d<=9;d++){
        for(int e = 1;e<=9;e++){
            for(int f = 1;f<=9;f++){
                for(int g = 1;g<=9;g++){
                    for(int h = 1;h<=9;h++){
                        permute(d,e,f,g,h, cyph, &ct,tid);
                    }
                }
            }
        }
    }
    printf("\n... Found %d solutions.\n", ct);
    return 0;
}
void *crack_thread2(void *arg) {
	 Params p;
    char *current;
    int i = 1;
    char ret[200];
    char cmd[400];
    FILE *Pipe;
    while (1) {
        current = enigma(p.cyph,&p);
            if (strcasestr(ret, permuteAll(p.cyph,arg)) != NULL) {
                strcpy(password_good, current);
                xmlMutexLock(finishedMutex);
                finished = 1;
                printf("GOOD: Message cracked: '%s'\n", current);
                xmlMutexUnlock(finishedMutex);
                savestatus();
                break;
            }

        pclose(Pipe);
        xmlMutexLock(finishedMutex);
        counter++;

        if (finished != 0) {
            xmlMutexUnlock(finishedMutex);
            break;
        }

        xmlMutexUnlock(finishedMutex);
        free(current);
    }
}

void *permuteAX(void *arg)
{
    pthread_t th[13];
    unsigned int i;

    for (i = 0; i < (unsigned int)arg; i++) {
        (void) pthread_create(&th[i], NULL, crack_thread2, arg);
    }

    (void) pthread_create(&th[12], NULL, status_thread, NULL);

    for (i = 0; i < (unsigned int)arg; i++) {
        (void) pthread_join(th[i], NULL);
    }

    (void) pthread_join(th[12], NULL);
}


/*once combination of five possible wheels*/
int permuteOnce(int a, int b, int c, int d, int e, char *cyph,struct _opaque_pthread_t *tid)
{
    int ct = 0;
    permute(a, b, c, d, e, cyph, &ct,tid);
    printf("\n... Found %d solutions.\n", ct);
    return 0;
}

void *crack_thread1(void *arg) {
	 Params p;
    char *current;
    int i = 1;
    char ret[200];
    char cmd[400];
    FILE *Pipe;
    while (1) {
        current = enigma(p.cyph,&p);
            if (strcasestr(ret, permuteOnce(p.order[0],p.order[1],p.order[2],p.order[3],p.order[4],p.cyph,arg)) != NULL) {
                strcpy(password_good, current);
                xmlMutexLock(finishedMutex);
                finished = 1;
                printf("GOOD: Message cracked: '%s'\n", current);
                xmlMutexUnlock(finishedMutex);
                savestatus();
                break;
            }

        xmlMutexLock(finishedMutex);
        counter++;

        if (finished != 0) {
            xmlMutexUnlock(finishedMutex);
            break;
        }

        xmlMutexUnlock(finishedMutex);
        free(current);
    }
}

void *permuteOX(void *arg)
{
    pthread_t th[13];
    unsigned int i;

    for (i = 0; i < (unsigned int)arg; i++) {
        (void) pthread_create(&th[i], NULL, crack_thread1, arg);
    }

    (void) pthread_create(&th[12], NULL, status_thread, NULL);

    for (i = 0; i < (unsigned int)arg; i++) {
        (void) pthread_join(th[i], NULL);
    }

    (void) pthread_join(th[12], NULL); 
}

/*helper to read a character*/
char readCh(void)
{
    char c, ret = '\0';
  while((c = getchar()) != VK_RETURN)
  ret = c;
  return ret;
}
/*init the starting position*/
void initParams(Params *p)
{
    int i,k,j;
    char c,d,g;
    printf("d)efault or u)ser: ");
    c = readCh();
    if(c != 'u')
    {
        if(strcmp(nerd, "--option-1") == 0)
        {
#if defined(__APPLE__)
            strlcpy(ukw,(const char *)p->ref1,MSGC);
            strlcpy(nox,(const char *)p->notch1,MSGG);
#elif defined(__LINUX__) || defined(__WIN32__) || defined(__WIN64__) 
            strcpy(ukw,(const char *)p->ref1);
            strcpy(nox,(const char *)p->notch1);
#endif
        }
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
        while((d = getchar()) != VK_RETURN)
        {
            p->plug[i] = d;
            i++;
        }
        p->plug[i] = '\0';
        
    }
    else
    {
        printf("Reflector: ");
        k = 0;
        while((d = getchar()) != VK_RETURN && (strlen(ukw) != (26)))
        {
            ukw[k] = d;
            k++;
        }
        ukw[k] = '\0';
        
        printf("NOTCH: ");
        j = 0;
        while((g = getchar()) != VK_RETURN && (strlen(nox) != (5)))
        {
            nox[j] = g;
            j++;
        }
        nox[j] = '\0';
        
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
        while((c = getchar()) != VK_RETURN && (strlen(p->plug) != (10)))
        {
            p->plug[i] = c;
            i++;
        }
        p->plug[i] = '\0';
        
    }
    printf("\x1B[33mWheels\x1B[39m \x1B[32m %d %d %d %d %d \x1B[39m \x1B[33mStart\x1B[39m \x1B[32m %c %c %c %c %c \x1B[39m \x1B[33mRings\x1B[39m \x1B[32m %c %c %c %c %c \x1B[39m Stecker \"\x1B[32m%s\x1B[39m\"\n",
           p->order[0], p->order[1], p->order[2], p->order[3], p->order[4],
           p->pos[0], p->pos[1], p->pos[2], p->pos[3], p->pos[4],
           p->rings[0], p->rings[1], p->rings[2], p->rings[3], p->rings[4], p->plug);
}

void sbfParams(Params *p)
{
    int i,j,k,l,m;
    char f,a,b,d,g,h;
    printf("d)efault or u)ser: ");
    f = readCh();
    if(f != 'u')
    {
        if(strcmp(nerd, "--option-1b") == 0)
        {
#if defined(__APPLE__)
            strlcpy(ukw,(const char *)p->ref1,MSGC);
            strlcpy(nox,(const char *)p->notch1,MSGG);
#elif defined(__LINUX__) || defined(__WIN32__) || defined(__WIN64__)
            strcpy(ukw,(const char *)p->ref1);
            strcpy(nox,(const char *)p->notch1);
#endif
        }
        for(i = 0; i < 5; i++)
        {
            printf("Wheel %d: ", i + 1);
            p->order[i] = (readCh() - 48);
        }
        
        printf("Message: ");
        i = 0;
        while((a = getchar()) != VK_RETURN && (strlen(p->cyph) != (MSGLEN - 1)))
        {
            p->cyph[i] = a;
            i++;
        }
        p->cyph[i] = '\0';
        
        printf("Dict: ");
        l = 0;
        while((b = getchar()) != VK_RETURN)
        {
            framex[l] = b;
            l++;
        }
        framex[l] = '\0';
        
        printf("Threads (1-12): ");
        m = 0;
        while((h = getchar()) != VK_RETURN)
        {
            chad[m] = h;
            m++;
        }
        chad[m] = '\0';
        
    }
    else
    {
        printf("Reflector: ");
        j = 0;
        while((d = getchar()) != VK_RETURN && (strlen(ukw) != (26)))
        {
            ukw[j] = d;
            j++;
        }
        ukw[j] = '\0';
        printf("NOTCH: ");
        k = 0;
        while((g = getchar()) != VK_RETURN && (strlen(nox) != (5)))
        {
            nox[k] = g;
            k++;
        }
        nox[k] = '\0';
        for(i = 0; i < 5; i++)
        {
            printf("Wheel %d: ", i + 1);
            p->order[i] = (readCh() - 48);
        }
        printf("Message: ");
        i = 0;
        while((a = getchar())  != VK_RETURN && (strlen(p->cyph) != (MSGLEN - 1)))
        {
            p->cyph[i] = a;
            i++;
        }
        p->cyph[i] = '\0';
        printf("Dict: ");
        l = 0;
        while((b = getchar())  != VK_RETURN)
        {
            framex[l] = b;
            l++;
        }
        framex[l] = '\0';
        
        printf("Threads (1-12): ");
        m = 0;
        while((h = getchar()) != VK_RETURN)
        {
            chad[m] = h;
            m++;
        }
        chad[m] = '\0';
    }
        //dict(p->crib);
        printf("\x1B[33mWheels\x1B[39m \x1B[32m %d %d %d %d %d \x1B[39m \x1B[33mMessage\x1B[39m\x1B[32m %s \x1B[39m\x1B[33mDict\x1B[39m \x1B[32m %s \x1B[39m\n",
               p->order[0], p->order[1], p->order[2], p->order[3], p->order[4],p->cyph,framex);
        int core = atoi(chad);
        if(core < 1 || core > 13) {
            perror(chad);
            exit(2);
        }
    
        srand(time(0));
        int result;
        //int status;
        
        result = pipe (fds);
        if (result == -1){
            perror("pipe");
            exit(2);
        }
        permuteOX((void*)core);
}

void bfParams(Params *p)
{
    int i,j,k,l,m;
    char c,a,b,d,g,h;
    printf("d)efault or u)ser: ");
    c = readCh();
    if(c != 'u')
    {
        if(strcmp(nerd, "--option-1a") == 0)
        {
#if defined(__APPLE__)
            strlcpy(ukw,(const char *)p->ref1,MSGC);
            strlcpy(nox,(const char *)p->notch1,MSGG);
#elif defined(__LINUX__) || defined(__WIN32__) || defined(__WIN64__)
            strcpy(ukw,(const char *)p->ref1);
            strcpy(nox,(const char *)p->notch1);
#endif
        }
        printf("Message: ");
        i = 0;
        while((a = getchar())  != VK_RETURN && (strlen(p->cyph) != (MSGLEN - 1)))
        {
            p->cyph[i] = a;
            i++;
        }
        p->cyph[i] = '\0';
        
        printf("Dict: ");
        l = 0;
        while((b = getchar()) != VK_RETURN)
        {
            framex[l] = b;
            l++;
        }
        framex[l] = '\0';
        
        printf("Threads (1-12): ");
        m = 0;
        while((h = getchar()) != VK_RETURN)
        {
            chad[m] = h;
            m++;
        }
        chad[m] = '\0';
        
    }
    else
    {
        printf("Reflector: ");
        j = 0;
        while((d = getchar()) !=VK_RETURN && (strlen(ukw) != (26)))
        {
            ukw[j] = d;
            j++;
        }
        ukw[j] = '\0';
        printf("NOTCH: ");
        k = 0;
        while((g = getchar()) != VK_RETURN && (strlen(nox) != (5)))
        {
            nox[k] = g;
            k++;
        }
        nox[k] = '\0';
        printf("Message: ");
        i = 0;
        while((a = getchar()) != VK_RETURN && (strlen(p->cyph) != (MSGLEN - 1)))
        {
            p->cyph[i] = a;
            i++;
        }
        p->cyph[i] = '\0';
        printf("Dict: ");
        l = 0;
        while((b = getchar()) != VK_RETURN)
        {
            framex[l] = b;
            l++;
        }
        framex[l] = '\0';
        
        printf("Threads (1-12): ");
        m = 0;
        while((h = getchar()) != VK_RETURN)
        {
            chad[m] = h;
            m++;
        }
        chad[m] = '\0';
    }
        //dict(p->crib);
        printf("\x1B[33mMessage\x1B[39m\x1B[32m %s \x1B[39m\x1B[33mDict\x1B[39m \x1B[32m %s \x1B[39m\n",
               p->cyph, framex);
        int core = atoi(chad);
        if(core < 1 || core > 13) {
            perror(chad);
            exit(2);
        }
        srand(time(0));
        int result;
        //int status;
        
        result = pipe (fds);
        if (result == -1){
            perror("pipe");
            exit(2);
        }
        permuteAX((void*)core);
}

/********************************************MAIN*********************************************/
int main(int argc, char* argv[]) {
    Params p;
    xmlInitThreads();
    pwdMutex = xmlNewMutex();
    finishedMutex = xmlNewMutex();
    
    if(argc < 2 || argc > 2){ /*main case*/
          printf("\x1b[32m");
          printf("\nOption usage: %s --help\n\n",argv[0]);
          printf("\x1b[0m");
          return 1;
      }

        if(strcmp(argv[1], "--version") == 0)
        {
            printf("Version\n\n\t%s\n\n",Versionx());
            exit(0);
        }
        if(strcmp(argv[1], "--help") == 0)
        {
            printf("Help\n\n\t\x1B[33m--option-1a\x1B[39m = Enigma Crack Algo\n\t\x1B[33m--option-1b\x1B[39m = Enigma Crack Once Algo\n\t\x1B[33m--option-1\x1B[39m = Enigma 5 Rotor Calculator\n\x1B[33m\n\t\x1B[33m--version\x1B[39m = Version\n\n");
            exit(0);
        }
        char a1,b2,c3,d4,e5,y,x;
        int a,b,c,d,e;
        printf("s)aved or r)emote: ");
        y = readCh();
        if(y != 'r')
        {
        flag = 0;
        printf("Config File: ");
        a = 0;
        while((a1 = getchar()) != VK_RETURN)
        {
            flames[a] = a1;
            a++;
        }
        flames[a] = '\0';
        
        configmain(&p,flames);
        }
        else
        {
            printf("p)roxy or d)irect Connection: ");
            x = readCh();
            if(x != 'p')
            {
            flag = 1;
            c = 0;
            printf("Config Host (domain): ");
            while((c3 = getchar()) != VK_RETURN)
            {
            host[c] = c3;
            c++;
            }
            host[c] = '\0';
            d = 0;
            printf("Config Host Port (443): ");
            while((d4 = getchar()) != VK_RETURN)
            {
            port[d] = d4;
            d++;
            }
            port[d] = '\0';
            e = 0;
            printf("Config Page (/gordon.php): ");
            while((e5 = getchar()) != VK_RETURN)
            {
            page[e] = e5;
            e++;
            }
            page[e] = '\0';
            
            connection_handler_d(&p,host,port,page);
            }
            else {
            flag = 1;
            printf("Config Proxy (ip): ");
            a = 0;
            while((a1 = getchar()) != VK_RETURN)
            {
                proxy[a] = a1;
                a++;
            }
            proxy[a] = '\0';
            printf("Config Proxy Port (8080): ");
            b = 0;
            while((b2 = getchar()) != VK_RETURN)
            {
                proxyport[b] = b2;
                b++;
            }
            proxyport[b] = '\0';
            c = 0;
            printf("Config Host (domain): ");
            while((c3 = getchar()) != VK_RETURN)
            {
                host[c] = c3;
                c++;
            }
            host[c] = '\0';
        /*d = 0;
        printf("Config Host Port (443): ");
        while((d4 = getchar()) != '\n')
        {
            port[d] = d4;
            d++;
        }
        port[d] = '\0';
        */
            e = 0;
            printf("Config Page (/gordon.php): ");
            while((e5 = getchar()) != VK_RETURN)
            {
                page[e] = e5;
                e++;
            }
            page[e] = '\0';
        
            connection_handler(&p,proxy,proxyport,host,page);
            }
        }
            if(strcmp(argv[1], "--option-1a") == 0)
            {
                strcpy(nerd,argv[1]);
                printf("Option 1a\n");
                bfParams(&p);
                //permuteAll(p.cyph);
            }
            if(strcmp(argv[1], "--option-1b") == 0)
            {
                strcpy(nerd,argv[1]);
                printf("Option 1b\n");
                sbfParams(&p);
                //permuteOnce(p.order[0], p.order[1], p.order[2], p.order[3], p.order[4],p.cyph);
            }
            if(strcmp(argv[1], "--option-1") == 0)
            {
            strcpy(nerd,argv[1]);
            printf("Enigma\n");
            initParams(&p);
            cypher(p);
            }
        
    return 0;
}
