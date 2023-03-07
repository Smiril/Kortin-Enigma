//
//  addx.h
//  enigma
//
//  Created by Smiril on 29.01.23.
//  Copyright © 2022 Smiril. All rights reserved.
//

#ifndef addx_h
#define addx_h

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

#define IN_RANK 0
#define MAXLINEX 1000000
#define MSGY 1320
#define MSGLEN 4096
#define MSGX 14
#define MSGXX 1
#define MSGC 26
#define MSGP 26
#define MSGG 5
#define TO 'Z'

#define PROGNAME "Enigma"

#endif /* addx_h */
