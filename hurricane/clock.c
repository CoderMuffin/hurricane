#ifndef HC_CLOCK
#define HC_CLOCK

#include <sys/time.h>
#include <stdlib.h>

typedef struct hc_clock {
    struct timeval last_time;
} hc_clock;

void hc_clock_new(hc_clock *clock) {
    gettimeofday(&clock->last_time, NULL);
}

double hc_clock_step(hc_clock *clock) {
    struct timeval now;
    gettimeofday(&now, NULL);
    double delta = (now.tv_sec - clock->last_time.tv_sec) + (now.tv_usec - clock->last_time.tv_usec) / 1000000.0;
    clock->last_time = now;
    return delta;
}

#endif
