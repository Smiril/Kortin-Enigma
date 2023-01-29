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

#include "add.h"

#define CA_PRIVATE_IMPLEMENTATION

void configmain(main_ctx_t *main_ctx,char *docname) {
   
    xmlDocPtr       doc;
    xmlNodePtr      cur;
    xmlChar         *uri;
    int count = 0;
    int count1 = 0;
    //int count2 = 0;
    char *config[4][7] = { {"rotor1", "rotor2", "rotor3", "rotor4", "rotor5"} };
    
    printf("Loading config file: %s\n",docname);
    FILE *fp;
    fp = fopen(docname,"r");
    struct stat st;
    fstat(fileno(fp), &st);
    size_t size = st.st_size;
  
    if (522 != size) {
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
      // move to the next attribute
      attr = attr->next;
    }

    // go through the child's children
    xmlNode* gchild = child->children->next;
    while (gchild != NULL) {
            if (xmlStrcmp(gchild->name, (const xmlChar *)config[count++])) {
                if((uri =  xmlGetProp(gchild,(const xmlChar *)"name"))) {
                    strcpy(main_ctx->rotor[count1++],(const char *)uri);
                    //printf("%s\n",uri);
                    xmlFree(uri);
                }
            }
            else if (xmlStrcmp(gchild->name, (const xmlChar *)"xmlref")) {
                if((uri =  xmlGetProp(gchild,(const xmlChar *)"name"))) {
                    strcpy(main_ctx->ref1,(const char *)uri);
                    //printf("%s\n",uri);
                    xmlFree(uri);
                }
            }
            else if (xmlStrcmp(gchild->name, (const xmlChar *)"xmlnotch")) {
                if((uri =  xmlGetProp(gchild,(const xmlChar *)"name"))) {
                    strcpy(main_ctx->notch1,(const char *)uri);
                    //printf("%s\n",uri);
                    xmlFree(uri);
                }
            }
        gchild = gchild->next->next;
    }
    xmlFreeDoc(doc);
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

int is_valid_ip(char *ip_str)
{
    int num, dots = 0;
    char *ptr;
 
    if (ip_str == NULL)
        return 0;
 
    ptr = strtok(ip_str, DELIM);
 
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
            ptr = strtok(NULL, DELIM);
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

int create_tcp_socket(){
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
  return socket_desc;
}

int create_udp_socket(){
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
  return socket_desc;
}

//note, to allow root to use icmp sockets, run:
//sysctl -w net.ipv4.ping_group_range="0 0"

int create_icmp_socket(){
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
  return socket_desc;
}

int create_igmp_socket(){
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
  return socket_desc;
}

int create_raw_socket(){
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
  return socket_desc;
}

char *get_ip(char *host){
    struct hostent *hent;
    int iplen = 15;
    char *ipx = (char *)malloc(iplen+1);
    memset(ipx,0,iplen+1);
    
    if((hent = gethostbyname(host)) != 0){
        perror("can not get ip");
    }
    
    if((inet_ntop(AF_INET,(void *)hent->h_addr_list[0],ipx,iplen)) != 0){
        perror("can not reslove host");
    }
    
    return ipx;
}

char *d_build_get_query_igmp(char *host,char *page) {
    char *query;
    char *getpage = page;
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


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
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


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
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


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
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


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
    char *tpl = "GET /%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


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
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


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
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


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
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


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
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


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
    char *tpl = "CONNECT %s:%s/%s HTTP\\1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close:\r\n";


    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }

    query = (char *)malloc(strlen(proxy)+strlen((const char *)proxyport)+strlen(getpage)+strlen(host)+strlen(USERAGENT)+strlen(tpl)-6);
    sprintf(query,tpl,proxy,proxyport,getpage,host,USERAGENT);
            
    return query;
}

void *connection_handler_d(main_ctx_t *main_ctx,char *host,char *port,char *page){
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
    else if( strcmp (PROTO,"ip") == 0){
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
    
#if !defined(__WIN32__) && !defined(__WIN64__)
    is_valid_ip(ip)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
    is_valid_ip(ip)? printf("Valid\n"): printf("Not valid\n");
#endif

    fprintf(stderr,"connect to %s ",ip);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(atoi(port));
    sa.sin_addr.s_addr = INADDR_ANY;
    
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    remote->sin_port = htons(atoi(port));
    tmpresx = inet_pton(AF_INET, host, (void *)(&(remote->sin_addr.s_addr)));
    
#if !defined(__WIN32__) && !defined(__WIN64__)
    is_valid_ip(ip)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
    is_valid_ip(ip)? printf("Valid\n"): printf("Not valid\n");
#endif
    
    int res;
    
    if( ( res = connect(sock2,(struct sockaddr *)remote,sizeof(struct sockaddr)) ) < 0 | errno != EINPROGRESS){
        perror("could not connect");
        exit(1);
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

    fprintf(stderr,"Query is:\n<<START>>\n%s\n<<END>>\n",get);

    int sent = 0;
    if( strcmp (SERVICE,"https") == 0){

            tmpres = SSL_write(ssl,get+sent,strlen(get)-sent);
            if(tmpres == 0){
                perror("can not send query");
                exit(1);
            }
        
    }
    
    if( strcmp (SERVICE,"http") == 0){
        
            tmpres = send(sock2,get+sent,strlen(get)-sent,0);
            if(tmpres == 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    memset(buf,0,sizeof(buf));
    
    int htmlstart = 0;
    char *htmlcontent;


    if( strcmp (SERVICE,"https") == 0){
        while((tmpres = SSL_read(ssl,buf,BUFSIZ)) > 0){
            if(htmlstart == 0){
                htmlcontent = strstr(buf,"\r\n\r\n");
                htmlstart = 1;
                htmlcontent += 4;
            }else{
                htmlcontent = buf ;
            }
            
            if(htmlstart){
                strcpy(flex,htmlcontent);
            }
            
            memset(buf,0,tmpres);
            
            if(tmpres == 0){
                perror("Error reciving data");
            }
            else {
                FILE *rp = fopen("/usr/local/share/enigma/remote.xml","w");
                fprintf(rp,"%s",flex);
                fclose(rp);
                configmain(main_ctx,"/usr/local/share/enigma/remote.xml");
            }
        }
    }
    
    if( strcmp (SERVICE,"http") == 0){
        while((tmpres = recv(sock2,buf,BUFSIZ,0)) == 0){
            if(htmlstart == 0){
                htmlcontent = strstr(buf,"\r\n");
            
                if(htmlcontent == 0){
                    htmlstart = 1;
                    htmlcontent += 4;
                }else{
                    htmlcontent = buf;
                }
                
                if(htmlstart){
                    strcpy(flex,htmlcontent);
                }
                
                memset(buf,0,tmpres);
            }
        
            if(tmpres == 0){
                perror("error reciving data");
            }
            else {
                FILE *rp = fopen("/usr/local/share/enigma/remote.xml","w");
                fprintf(rp,"%s",flex);
                fclose(rp);
                configmain(main_ctx,"/usr/local/share/enigma/remote.xml");
            }
        }
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

void *connection_handler(main_ctx_t *main_ctx,char *proxy,char *proxyport,char *host,char *port,char *page){
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
    else if( strcmp (PROTO,"ip") == 0){
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
    fprintf(stderr,"go from %s ",ipp);
#if !defined(__WIN32__) && !defined(__WIN64__)
    is_valid_ip(ipp)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
    is_valid_ip(ipp)? printf("Valid\n"): printf("Not valid\n");
#endif
    fprintf(stderr,"connect to %s ",ip);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(atoi(port));
    sa.sin_addr.s_addr = INADDR_ANY;
    
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    remote->sin_port = htons(atoi(proxyport));
    tmpresx = inet_pton(AF_INET, proxy, (void *)(&(remote->sin_addr.s_addr)));
    
#if !defined(__WIN32__) && !defined(__WIN64__)
    is_valid_ip(ip)? printf("\x1B[32mValid\x1B[39m\n"): printf("\x1B[33mNot valid\x1B[39m\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
    is_valid_ip(ip)? printf("Valid\n"): printf("Not valid\n");
#endif
    
    int res;
    
    if( ( res = connect(sock2,(struct sockaddr *)remote,sizeof(struct sockaddr)) ) < 0 | errno != EINPROGRESS){
        perror("could not connect");
        exit(1);
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
#if !defined(__WIN32__) && !defined(__WIN64__)
    fprintf(stderr,"Query is:\n\x1B[32m<<START>>\x1B[39m\n\x1B[33m%s\x1B[39m\n\x1B[32m<<END>>\x1B[39m\n",get);
#elif !defined(__APPLE__) && !defined(__LINUX__)
    fprintf(stderr,"Query is:\n<<START>>\n%s\n<<END>>\n",get);
#endif
    int sent = 0;
    if( strcmp (SERVICE,"https") == 0){

            tmpres = SSL_write(ssl,get+sent,strlen(get)-sent);
            if(tmpres == 0){
                perror("can not send query");
                exit(1);
            }
        
    }
    
    if( strcmp (SERVICE,"http") == 0){
        
            tmpres = send(sock2,get+sent,strlen(get)-sent,0);
            if(tmpres == 0){
                perror("can not send query");
                exit(1);
            }
            sent += tmpres;
        
    }
    
    memset(buf,0,sizeof(buf));
    
    int htmlstart = 0;
    char *htmlcontent;


    if( strcmp (SERVICE,"https") == 0){
        while((tmpres = SSL_read(ssl,buf,BUFSIZ)) > 0){
            if(htmlstart == 0){
                htmlcontent = strstr(buf,"\r\n\r\n");
                htmlstart = 1;
                htmlcontent += 4;
            }else{
                htmlcontent = buf ;
            }
            
            if(htmlstart){
                strcpy(flex,htmlcontent);
            }
            
            memset(buf,0,tmpres);
            
            if(tmpres == 0){
                perror("Error reciving data");
            }
            else {
                FILE *rp = fopen("/usr/local/share/enigma/remote.xml","w");
                fprintf(rp,"%s",flex);
                fclose(rp);
                configmain(main_ctx,"/usr/local/share/enigma/remote.xml");
            }
        }
    }
    
    if( strcmp (SERVICE,"http") == 0){
        while((tmpres = recv(sock2,buf,BUFSIZ,0)) == 0){
            if(htmlstart == 0){
                htmlcontent = strstr(buf,"\r\n");
            
                if(htmlcontent == 0){
                    htmlstart = 1;
                    htmlcontent += 4;
                }else{
                    htmlcontent = buf;
                }
                
                if(htmlstart){
                    strcpy(flex,htmlcontent);
                }
                
                memset(buf,0,tmpres);
            }
        
            if(tmpres == 0){
                perror("error reciving data");
            }
            else {
                FILE *rp = fopen("/usr/local/share/enigma/remote.xml","w");
                fprintf(rp,"%s",flex);
                fclose(rp);
                configmain(main_ctx,"/usr/local/share/enigma/remote.xml");
            }
        }
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
        if (main_ctx->pos[0]==main_ctx->notch1[main_ctx->order[0]-1])
        {
            main_ctx->pos[1]++;
            if (main_ctx->pos[1]>'Z')
                main_ctx->pos[1] -= 26;
            if (main_ctx->pos[0]==main_ctx->notch1[main_ctx->order[1]-1])
            {
                main_ctx->pos[1]++;
                if (main_ctx->pos[1]>'Z')
                    main_ctx->pos[1] -= 26;
                if (main_ctx->pos[0]==main_ctx->notch1[main_ctx->order[2]-1])
                {
                    main_ctx->pos[1]++;
                    if (main_ctx->pos[1]>'Z')
                        main_ctx->pos[1] -= 26;
                    if (main_ctx->pos[0]==main_ctx->notch1[main_ctx->order[3]-1])
                    {
                        main_ctx->pos[1]++;
                        if (main_ctx->pos[1]>'Z')
                            main_ctx->pos[1] -= 26;
                        if (main_ctx->pos[0]==main_ctx->notch1[main_ctx->order[4]-1])
                        {
                            main_ctx->pos[1]++;
                            if (main_ctx->pos[1]>'Z')
                                main_ctx->pos[1] -= 26;
                            flag=f;
            /* Set flag if second rotor reached notch */
                            if (main_ctx->pos[1]==main_ctx->notch1[main_ctx->order[0]-1])
                            {
                                main_ctx->pos[2]++;
                                if (main_ctx->pos[2]>'Z')
                                    main_ctx->pos[2] -= 26;
                                if (main_ctx->pos[1]==main_ctx->notch1[main_ctx->order[1]-1])
                                {
                                    main_ctx->pos[2]++;
                                    if (main_ctx->pos[2]>'Z')
                                        main_ctx->pos[2] -= 26;
                                    if (main_ctx->pos[1]==main_ctx->notch1[main_ctx->order[2]-1])
                                    {
                                        main_ctx->pos[2]++;
                                        if (main_ctx->pos[2]>'Z')
                                            main_ctx->pos[2] -= 26;
                                        if (main_ctx->pos[1]==main_ctx->notch1[main_ctx->order[3]-1])
                                        {
                                            main_ctx->pos[2]++;
                                            if (main_ctx->pos[2]>'Z')
                                                main_ctx->pos[2] -= 26;
                                            if (main_ctx->pos[1]==main_ctx->notch1[main_ctx->order[4]-1])
                                            {
                                                main_ctx->pos[2]++;
                                                if (main_ctx->pos[2]>'Z')
                                                    main_ctx->pos[2] -= 26;
                                                flag=f;
                                                if (main_ctx->pos[2]==main_ctx->notch1[main_ctx->order[0]-1])
                                                {
                                                    main_ctx->pos[3]++;
                                                    if (main_ctx->pos[3]>'Z')
                                                        main_ctx->pos[3] -= 26;
                                                    if (main_ctx->pos[2]==main_ctx->notch1[main_ctx->order[1]-1])
                                                    {
                                                        main_ctx->pos[3]++;
                                                        if (main_ctx->pos[3]>'Z')
                                                            main_ctx->pos[3] -= 26;
                                                        if (main_ctx->pos[2]==main_ctx->notch1[main_ctx->order[2]-1])
                                                        {
                                                            main_ctx->pos[3]++;
                                                            if (main_ctx->pos[3]>'Z')
                                                                main_ctx->pos[3] -= 26;
                                                            if (main_ctx->pos[2]==main_ctx->notch1[main_ctx->order[3]-1])
                                                            {
                                                                main_ctx->pos[3]++;
                                                                if (main_ctx->pos[3]>'Z')
                                                                    main_ctx->pos[3] -= 26;
                                                                if (main_ctx->pos[2]==main_ctx->notch1[main_ctx->order[4]-1])
                                                                {
                                                                    main_ctx->pos[3]++;
                                                                    if (main_ctx->pos[3]>'Z')
                                                                        main_ctx->pos[3] -= 26;
                                                                    flag=f;
                                                                    if (main_ctx->pos[3]==main_ctx->notch1[main_ctx->order[0]-1])
                                                                    {
                                                                        main_ctx->pos[4]++;
                                                                        if (main_ctx->pos[4]>'Z')
                                                                            main_ctx->pos[4] -= 26;
                                                                        if (main_ctx->pos[3]==main_ctx->notch1[main_ctx->order[1]-1])
                                                                        {
                                                                            main_ctx->pos[4]++;
                                                                            if (main_ctx->pos[4]>'Z')
                                                                                main_ctx->pos[4] -= 26;
                                                                            if (main_ctx->pos[3]==main_ctx->notch1[main_ctx->order[2]-1])
                                                                            {
                                                                                main_ctx->pos[4]++;
                                                                                if (main_ctx->pos[4]>'Z')
                                                                                    main_ctx->pos[4] -= 26;
                                                                                if (main_ctx->pos[3]==main_ctx->notch1[main_ctx->order[3]-1])
                                                                                {
                                                                                    main_ctx->pos[4]++;
                                                                                    if (main_ctx->pos[4]>'Z')
                                                                                        main_ctx->pos[4] -= 26;
                                                                                    if (main_ctx->pos[3]==main_ctx->notch1[main_ctx->order[4]-1])
                                                                                    {
                                                                                        main_ctx->pos[4]++;
                                                                                        if (main_ctx->pos[4]>'Z')
                                                                                            main_ctx->pos[4] -= 26;
                                                                                        flag=f;
                                                                                        if (main_ctx->pos[4]==main_ctx->notch1[main_ctx->order[0]-1])
                                                                                        {
                                                                                            if (main_ctx->pos[4]==main_ctx->notch1[main_ctx->order[1]-1])
                                                                                            {
                                                                                                if (main_ctx->pos[4]==main_ctx->notch1[main_ctx->order[2]-1])
                                                                                                {
                                                                                                    if (main_ctx->pos[4]==main_ctx->notch1[main_ctx->order[3]-1])
                                                                                                    {
                                                                                                        if (main_ctx->pos[4]==main_ctx->notch1[main_ctx->order[4]-1])
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

            c=main_ctx->rotor[main_ctx->order[i]-1][c-'A'];

            c += main_ctx->rings[i]-'A';
            if (c>'Z')
                c -= 26;

            c -= main_ctx->pos[i]-'A';
            if (c<'A')
                c += 26;
        }

        /*  Reflecting rotor */

        c=main_ctx->ref1[c-'A'];
    
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
                if (main_ctx->rotor[main_ctx->order[i-1]-1][j]==c)
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
int cypher(main_ctx_t main_ctx)
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
    return 0;
}

int rotate(main_ctx_t *main_ctx,int a, int b, int c, int d, int e, char *cyph, char *plug, int *ct)
{
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
                                        strncpy(fff, enigma(cyph, main_ctx), strlen(fff));
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
#if !defined(__WIN32__) && !defined(__WIN64__)
                                                    printf("\x1B[33mWheels\x1B[39m \x1B[32m%d %d %d %d %d\x1B[39m \x1B[33mStart\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mRings\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mStecker\x1B[39m \"\x1B[32m%s\x1B[39m\" TEXT: %s Rank: %d\n",
                                                           main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4],
                                                           main_ctx->pos[0], main_ctx->pos[1], main_ctx->pos[2], main_ctx->pos[3], main_ctx->pos[4],
                                                           main_ctx->rings[0], main_ctx->rings[1], main_ctx->rings[2], main_ctx->rings[3], main_ctx->rings[4], main_ctx->plug, fff, rank);
#elif !defined(__APPLE__) && !defined(__LINUX__)
                                                    printf("Wheels %d %d %d %d %d Start %c %c %c %c %c Ring %c %c %c %c %c Stecker \"%s\" TEXT: %s Rank: %d\n",
                                                           main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4],
                                                           main_ctx->pos[0], main_ctx->pos[1], main_ctx->pos[2], main_ctx->pos[3], main_ctx->pos[4],
                                                           main_ctx->rings[0], main_ctx->rings[1], main_ctx->rings[2], main_ctx->rings[3], main_ctx->rings[4], main_ctx->plug, fff, rank);
#endif
                                                    oldrank = rank;
                                                }
                                                
                                            } else continue;
                                            
                                        if(strcmp(cyph, enigma(fff, main_ctx)) == 0)
                                        {
#if !defined(__WIN32__) && !defined(__WIN64__)
                                            printf("\x1B[33mWheels\x1B[39m \x1B[32m%d %d %d %d %d\x1B[39m \x1B[33mStart\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mRings\x1B[39m \x1B[32m%c %c %c %c %c\x1B[39m \x1B[33mStecker\x1B[39m \"\x1B[32m%s\x1B[39m\"\nReflector: %s\nNOTCH: %s\n",
                                                   main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4],
                                                   main_ctx->pos[0], main_ctx->pos[1], main_ctx->pos[2], main_ctx->pos[3], main_ctx->pos[4],
                                                   main_ctx->rings[0], main_ctx->rings[1], main_ctx->rings[2], main_ctx->rings[3], main_ctx->rings[4], main_ctx->plug,main_ctx->ref1,main_ctx->notch1);
#elif !defined(__APPLE__) && !defined(__LINUX__)
                                            printf("Wheels %d %d %d %d %d Start %c %c %c %c %c Rings %c %c %c %c %c Stecker \"%s\"\nReflector: %s\nNOTCH: %s\n",
                                                   main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4],
                                                   main_ctx->pos[0], main_ctx->pos[1], main_ctx->pos[2], main_ctx->pos[3], main_ctx->pos[4],
                                                   main_ctx->rings[0], main_ctx->rings[1], main_ctx->rings[2], main_ctx->rings[3], main_ctx->rings[4], main_ctx->plug,main_ctx->ref1,main_ctx->notch1);
#endif
                                            printf("%s decoded -> %s\n",cyph,enigma(cyph, main_ctx));
                                            const time_t proc_stop = time (NULL);
                                            printf("Time elapsed : begin %ld - end %ld \n",proc_start,proc_stop);
                                            
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

int test(main_ctx_t *main_ctx,int a, int b, int c, int d, int e, char *cyph, int *ct) {
   
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
    return 0;
}

/*run on all permutations of wheels a, b, c, d, e*/
int permute(main_ctx_t *main_ctx,int a, int b, int c, int d, int e, char *cyph, int *ct)
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
    return 0;
}

void *reader(void *arg) {
 int *count = (int *)arg;
 //sleep (25);
  //Delay in starting the reading from the pipe
    int     result;
    
    while(1) {
        result = read (fds[0],&count,sizeof(count));
        if (result == -1) {
            perror("read");
            exit(3);
        }
    }
}

/*all combinations of five possible wheels*/
int permuteAll(main_ctx_t *main_ctx,char *cyph)
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
    return 0;
}

void *permuteAX(void *arg)
{
    main_ctx_t main_ctx;
    pthread_mutex_lock(&lock);              // lock
    int *fds = (int *)arg;
    int result;
    pthread_t t = pthread_self();
    //printf("created: %d\n", fds[1]);
    //pthread_detach(pthread_self());
    while(1) {
        result = write (fds[1], &t,sizeof(t));
        if (result == -1){
            perror ("write");
            exit (2);
        }
        
        if(!permuteAll(&main_ctx,main_ctx.cyph)){
            perror("main");
        }
    }
    pthread_mutex_unlock(&lock);            //release lock
    pthread_exit(NULL);                     //exit from child thread
}
/*once combination of five possible wheels*/
int permuteOnce(main_ctx_t *main_ctx,int a, int b, int c, int d, int e, char *cyph)
{
    int ct = 0;
    permute(main_ctx,a, b, c, d, e, cyph, &ct);
    printf("\n... Found %d solutions.\n", ct);
    return 0;
}
void *permuteOX(void *arg)
{
    main_ctx_t main_ctx;
    pthread_mutex_lock(&lock);
    int *fds = (int *)arg;
    int result;
    pthread_t t = pthread_self();
    //printf("created: %d\n", fds[1]);
    //pthread_detach(pthread_self());
    while(1) {
        result = write (fds[1], &t,sizeof(t));
        if (result == -1){
            perror ("write");
            exit (2);
        }
        
        if(!permuteOnce(&main_ctx,main_ctx.order[0], main_ctx.order[1],main_ctx.order[2], main_ctx.order[3], main_ctx.order[4],main_ctx.cyph)) {
            perror("main");
        }
    }
    pthread_mutex_unlock(&lock);            //release lock
    pthread_exit(NULL);                     //exit from child thread
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
        for(i = 0; i < 5; i++)
        {
          printf("Wheel %d: ", i + 1);
          main_ctx->order[i] = (readCh() - 48);
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
        main_ctx->ref1[k] = d;
        k++;
        }
        main_ctx->ref1[k] = '\0';
        
        printf("NOTCH: ");
        j = 0;
        while((g = getchar()) != '\n')
        {
        main_ctx->notch1[j] = g;
        j++;
        }
        main_ctx->notch1[j] = '\0';
        
        for(i = 0; i < 5; i++)
        {
          printf("Wheel %d: ", i + 1);
          main_ctx->order[i] = (readCh() - 48);
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
#if !defined(__WIN32__) && !defined(__WIN64__)
  printf("\x1B[33mWheels\x1B[39m \x1B[32m %d %d %d %d %d \x1B[39m \x1B[33mStart\x1B[39m \x1B[32m %c %c %c %c %c \x1B[39m \x1B[33mRings\x1B[39m \x1B[32m %c %c %c %c %c \x1B[39m Stecker \"\x1B[32m%s\x1B[39m\"\n\x1B[33mReflector\x1B[39m \x1B[32m %s \x1B[39m \x1B[33mNOTch\x1B[39m\x1B[32m %s \x1B[39m\n",
         main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4],
         main_ctx->pos[0], main_ctx->pos[1], main_ctx->pos[2], main_ctx->pos[3], main_ctx->pos[4],
         main_ctx->rings[0], main_ctx->rings[1], main_ctx->rings[2], main_ctx->rings[3], main_ctx->rings[4], main_ctx->plug,main_ctx->ref1,main_ctx->notch1);
#elif !defined(__APPLE__) && !defined(__LINUX__)
    printf("Wheels %d %d %d %d %d Start %c %c %c %c %c Rings %c %c %c %c %c Stecker \"%s"\nReflector %s NOTch %s \n",
           main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4],
           main_ctx->pos[0], main_ctx->pos[1], main_ctx->pos[2], main_ctx->pos[3], main_ctx->pos[4],
           main_ctx->rings[0], main_ctx->rings[1], main_ctx->rings[2], main_ctx->rings[3], main_ctx->rings[4], main_ctx->plug,main_ctx->ref1,main_ctx->notch1);
#endif
}

void sbfParams(main_ctx_t *main_ctx)
{
    int i,j,k,l,m;
    char f,a,b,d,g,h;
    printf("d)efault or u)ser: ");
    f = readCh();
    if(f != 'u')
    {
        for(i = 0; i < 5; i++)
        {
            printf("Wheel %d: ", i + 1);
            main_ctx->order[i] = (readCh() - 48);
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
        
        printf("Threads (1-10): ");
        m = 0;
        while((h = getchar()) != '\n')
        {
        chad[m] = h;
        l++;
        }
        chad[m] = '\0';
        
        //strcpy(framex,"/usr/local/share/enigma/german.txt");
    }
    else
    {
        printf("Reflector: ");
        j = 0;
        while((d = getchar()) != '\n')
        {
            main_ctx->ref1[j] = d;
            j++;
        }
        main_ctx->ref1[j] = '\0';
        printf("NOTCH: ");
        k = 0;
        while((g = getchar()) != '\n')
        {
        main_ctx->notch1[k] = g;
        k++;
        }
        main_ctx->notch1[k] = '\0';
        for(i = 0; i < 5; i++)
        {
            printf("Wheel %d: ", i + 1);
            main_ctx->order[i] = (readCh() - 48);
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
        
        printf("Threads (1-10): ");
        m = 0;
        while((h = getchar()) != '\n')
        {
        chad[m] = h;
        l++;
        }
        chad[m] = '\0';
        
        //strcpy(framex,"/usr/local/share/enigma/german.txt");
    }
#if !defined(__WIN32__) && !defined(__WIN64__)
    printf("\x1B[33mWheels\x1B[39m \x1B[32m %d %d %d %d %d \x1B[39m \x1B[33mMessage\x1B[39m\x1B[32m %s \x1B[39m\x1B[33mDict\x1B[39m \x1B[32m %s \x1B[39m\n\x1B[33mReflector\x1B[39m \x1B[32m %s \x1B[39m \x1B[33mNOTch\x1B[39m\x1B[32m %s \x1B[39m\n",
           main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4], main_ctx->cyph, framex,main_ctx->ref1,main_ctx->notch1);
#elif !defined(__APPLE__) && !defined(__LINUX__)
    printf("Wheels%d %d %d %d %d Message %s Dict %s \nReflector %s NOTch %s \n",
           main_ctx->order[0], main_ctx->order[1], main_ctx->order[2], main_ctx->order[3], main_ctx->order[4], main_ctx->cyph, framex,main_ctx->ref1,main_ctx->notch1);
#endif
    int core = atoi(chad);
    srand(time(0));
    int result;
    int status;

    result = pipe (fds);
    if (result < 0){
        perror("pipe");
        exit(1);
    }
    
    for (int i = 0; i < core; i++) {
        pthread_t tid = malloc(core + 1 * sizeof(pthread_t));
        //pthread_create(*(pthread_t**)&tid[i], NULL, reader, (void*)&fds[i]);
        pthread_create(*(pthread_t**)&tid[i], NULL, permuteOX, (void*)&fds[i]);
        printf("created: %llu\n", (unsigned long long)&tid[i]);
    }
   
    for (int i = 0; i < core; i++) {
        pthread_t tid = malloc(core + 1 * sizeof(pthread_t));
        read(fds[0], &tid[i], sizeof(tid[i]));
        printf("joining: %llu\n", (unsigned long long)&tid[i]);
        pthread_join(&tid[i], (void*)&status);
        printf("Return Thread: %d Value: %d\n",(int)&tid[i],(int)status);
    }
    //pthread_exit(0);
}

void bfParams(main_ctx_t *main_ctx)
{
    int i,j,k,l,m;
    char c,a,b,d,g,h;
    printf("d)efault or u)ser: ");
    c = readCh();
    if(c != 'u')
    {
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
        
        printf("Threads (1-10): ");
        m = 0;
        while((h = getchar()) != '\n')
        {
        chad[m] = h;
        l++;
        }
        chad[m] = '\0';
        
        //strcpy(framex,"/usr/local/share/enigma/german.txt");
    }
    else
    {
        printf("Reflector: ");
        j = 0;
        while((d = getchar()) != '\n')
        {
        main_ctx->ref1[j] = d;
        j++;
        }
        main_ctx->ref1[j] = '\0';
        printf("NOTCH: ");
        k = 0;
        while((g = getchar()) != '\n')
        {
        main_ctx->notch1[k] = g;
        k++;
        }
        main_ctx->notch1[k] = '\0';
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
        
        printf("Threads (1-10): ");
        m = 0;
        while((h = getchar()) != '\n')
        {
        chad[m] = h;
        l++;
        }
        chad[m] = '\0';

        //strcpy(framex,"/usr/local/share/enigma/german.txt");
    }
#if !defined(__WIN32__) && !defined(__WIN64__)
    printf("\x1B[33mMessage\x1B[39m\x1B[32m %s \x1B[39m\x1B[33mDict\x1B[39m \x1B[32m %s \x1B[39m\n\x1B[33mReflector\x1B[39m \x1B[32m %s \x1B[39m \x1B[33mNOTch\x1B[39m\x1B[32m %s \x1B[39m\n",
           main_ctx->cyph, framex,main_ctx->ref1,main_ctx->notch1);
#elif !defined(__APPLE__) && !defined(__LINUX__)
    printf("Message %s Dict %s \nReflector %s NOTch %s \n",
           main_ctx->cyph, framex,main_ctx->ref1,main_ctx->notch1);
#endif
    int core = atoi(chad);
    srand(time(0));
    int result;
    int status;

    result = pipe (fds);
    if (result < 0){
        perror("pipe");
        exit(1);
    }
    
    for (int i = 0; i < core; i++) {
        pthread_t tid = malloc(core + 1 * sizeof(pthread_t));
        //pthread_create(*(pthread_t**)&tid[i], NULL, reader, (void*)&fds[i]);
        pthread_create(*(pthread_t**)&tid[i], NULL, permuteAX, (void*)&fds[i]);
        printf("created: %llu\n", (unsigned long long)&tid[i]);
    }
   
    for (int i = 0; i < core; i++) {
        pthread_t tid = malloc(core + 1 * sizeof(pthread_t));
        read(fds[0], &tid[i], sizeof(tid[i]));
        printf("joining: %llu\n", (unsigned long long)&tid[i]);
        pthread_join(&tid[i], (void*)&status);
        printf("Return Thread: %d Value: %d\n",(int)&tid[i],(int)status);
    }
    //pthread_exit(0);
}


/********************************************MAIN*********************************************/
int main(int argc, char **argv) {
    main_ctx_t main_ctx;
    
    if(argc < 2 || argc > 2){ /*main case*/
#if !defined(__WIN32__) && !defined(__WIN64__)
          printf("\x1b[32m");
#endif
          printf("\nOption usage: %s --help\n",argv[0]);
#if !defined(__WIN32__) && !defined(__WIN64__)
          printf("\x1b[0m");
#endif
          return -1;
        }
    
        if(strcmp(argv[1], "--version") == 0)
        {
#if !defined(__WIN32__) && !defined(__WIN64__)
            printf("Version\n\n\t\x1B[35m%s\x1B[39m -  \x1B[32mT.E.D.\x1B[39m - \x1B[33mThe Enemy Dail\x1B[39m - Koenig Martin\n",PROGNAME);
#elif !defined(__APPLE__) && !defined(__LINUX__)
            printf("Version\n\n\t%s -  T.E.D. - The Enemy Dail - Koenig Martin\n",PROGNAME);
#endif
            return 0;
        }
        if(strcmp(argv[1], "--help") == 0)
        {
#if !defined(__WIN32__) && !defined(__WIN64__)
            printf("Help\n\n\t\x1B[33m--option-1a\x1B[39m = Enigma Crack Algo\n\t\x1B[33m--option-1b\x1B[39m = Enigma Crack Once Algo\n\t\x1B[33m--option-1\x1B[39m = Enigma 5 Rotor Calculator\n\t\x1B[33m--version\x1B[39m = Version\n\n");
#elif !defined(__APPLE__) && !defined(__LINUX__)
            printf("Help\n\n\t--option-1a = Enigma Crack Algo\n\t--option-1b = Enigma Crack Once Algo\n\t--option-1 = Enigma 5 Rotor Calculator\n\t--version = Version\n\n");
#endif
            return 0;
        }
    
        char a1,b2,c3,d4,e5,y,p;
        int a,b,c,d,e;
        printf("s)aved or r)emote: ");
        y = readCh();
        if(y != 'r')
        {
            flag = 0;
            printf("Config File: ");
            a = 0;
            while((a1 = getchar()) != '\n')
            {
                flames[a] = a1;
                a++;
            }
            flames[a] = '\0';
            
            configmain(&main_ctx,flames);
        }
        else
        {
            printf("p)roxy or d)irect Connection: ");
            p = readCh();
            if(p != 'p')
            {
                flag = 1;
                c = 0;
                printf("Config Host (domain): ");
                while((c3 = getchar()) != '\n')
                {
                host[c] = c3;
                c++;
                }
                host[c] = '\0';
                d = 0;
                printf("Config Host Port (443): ");
                while((d4 = getchar()) != '\n')
                {
                port[d] = d4;
                d++;
                }
                port[d] = '\0';
                e = 0;
                printf("Config Page (/gordon.php): ");
                while((e5 = getchar()) != '\n')
                {
                page[e] = e5;
                e++;
                }
                page[e] = '\0';
                
                connection_handler_d(&main_ctx,host,port,page);
            }
            else {
            flag = 1;
            printf("Config Proxy (ip): ");
            a = 0;
            while((a1 = getchar()) != '\n')
            {
                proxy[a] = a1;
                a++;
            }
            proxy[a] = '\0';
            printf("Config Proxy Port (8080): ");
            b = 0;
            while((b2 = getchar()) != '\n')
            {
                proxyport[b] = b2;
                b++;
            }
            proxyport[b] = '\0';
            c = 0;
            printf("Config Host (domain): ");
            while((c3 = getchar()) != '\n')
            {
                host[c] = c3;
                c++;
            }
            host[c] = '\0';
            d = 0;
            printf("Config Host Port (443): ");
            while((d4 = getchar()) != '\n')
            {
                port[d] = d4;
                d++;
            }
            port[d] = '\0';
            e = 0;
            printf("Config Page (/gordon.php): ");
            while((e5 = getchar()) != '\n')
            {
                page[e] = e5;
                e++;
            }
            page[e] = '\0';
            
            connection_handler(&main_ctx,proxy,proxyport,host,port,page);
            }
        }
    
        if(strcmp(argv[1], "--option-1a") == 0)
        {
            strcpy(nerd,argv[1]);
            printf("Option 1\n");
            bfParams(&main_ctx);
        }
        if(strcmp(argv[1], "--option-1b") == 0)
        {
            strcpy(nerd,argv[1]);
            printf("Option 1\n");
            sbfParams(&main_ctx);
        }
        if(strcmp(argv[1], "--option-1") == 0)
        {
            strcpy(nerd,argv[1]);
            printf("Enigma\n");
            initParams(&main_ctx);
            cypher(main_ctx);
        }
    
    return 0;
}
