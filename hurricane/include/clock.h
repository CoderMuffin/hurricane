#ifndef HC_CLOCK_H
#define HC_CLOCK_H

//macros
#include <time.h>
#include <sys/utime.h>
#include <sys/timeb.h>

//types
typedef struct hc_clock {
    struct timeb last_time;
} hc_clock;

//decls

//functions
void hc_clock_new(hc_clock *clock);
double hc_clock_step(hc_clock *clock);

#endif
