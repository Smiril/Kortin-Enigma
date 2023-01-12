//
//  addx.h
//
//  Created by Smiril on 10.01.23.
//  Copyright © 2022 Smiril. All rights reserved.
//

#ifndef addx_h
#define addx_h

#define MSGC 26
#define MSGG 5

// timer

#if defined(__APPLE__) && defined(MISSING_CLOCK_GETTIME)
typedef struct timeval    hc_timer_t;
#else
typedef struct timespec   hc_timer_t;
#endif

#endif /* addx_h */
