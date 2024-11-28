#ifndef HC_CLOCK_H
#define HC_CLOCK_H

#include <sys/time.h>

typedef struct hc_clock {
    struct timeval last_time;
} hc_clock;

void hc_clock_new(hc_clock *clock);
double hc_clock_step(hc_clock *clock);

#endif
