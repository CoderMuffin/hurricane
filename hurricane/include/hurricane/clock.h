#ifndef HC_CLOCK_H
#define HC_CLOCK_H

#include "fixed.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <stdint.h> // portable: uint64_t   MSVC: __int64 

    struct timeval {
        long tv_sec;
        long tv_usec;
    };

    struct timezone;
    
    int gettimeofday(struct timeval * tp, struct timezone * tzp);
#else
   // #include <sys/time.h>
#endif

typedef struct hc_clock {
    //struct timeval last_time;
    short x;
} hc_clock;

// Source - https://stackoverflow.com/a/26085827
// Posted by Michaelangel007, modified by community. See post 'Timeline' for change history
// Retrieved 2025-12-15, License - CC BY-SA 3.0

void hc_clock_new(hc_clock *clock);
fixed hc_clock_step(hc_clock *clock);

#endif
