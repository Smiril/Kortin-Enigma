//
//  addx.h
//  enigma
//
//  Created by Smiril on 20.07.22.
//  Copyright © 2022 Smiril. All rights reserved.
//

#ifndef addx_h
#define addx_h
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define IN_RANK 0
#define MAXLINEX 1000000
#define MSGY 1320
#define MSGLEN 161
#define MSGX 13
#define MSGC 26
#define MSGP 11
#define MSGG 5
#define TO 'Z'

#define PW_MIN              0
#define PW_MAX              256
#define PW_MAX_OLD          55

#define SALT_MIN            0
#define SALT_MAX            256
#define SALT_MAX_OLD        51

#define HCBUFSIZ_TINY       0x1000
#define HCBUFSIZ_SMALL      0x2000
#define HCBUFSIZ_LARGE      0x1000000

#define CPT_CACHE           0x20000
#define PARAMCNT            64
#define DEVICES_MAX         128
#define EXEC_CACHE          128
#define SPEED_CACHE         4096
#define SPEED_MAXAGE        4096
#define EXPECTED_ITERATIONS 10000
// there's no such thing in plain C, therefore all vector operation cannot work in this emu
// which is why VECT_SIZE is set to 1
typedef uint32_t uint4;

typedef pthread_t        hc_thread_t;
typedef pthread_mutex_t  hc_thread_mutex_t;
typedef sem_t            hc_thread_semaphore_t;

// timer
#if defined(__APPLE__) && defined(MISSING_CLOCK_GETTIME)
typedef struct timeval    hc_timer_t;
#else
typedef struct timespec   hc_timer_t;
#endif

#if defined (__APPLE__)

int               mtl_major;
int               mtl_minor;

int               device_physical_location;
int               device_location_number;
int               device_registryID;
int               device_max_transfer_rate;
int               device_is_headless;
int               device_is_low_power;
int               device_is_removable;

int               metal_warp_size;
#endif //__APPLE__

#endif /* addx_h */
