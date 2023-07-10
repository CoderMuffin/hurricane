#ifndef HC_CLOCK
#define HC_CLOCK

#include <time.h>
#include <sys/utime.h>
#include <sys/timeb.h>

typedef struct hc_clock {
    struct timeb last_time;
} hc_clock;

void hc_clock_new(hc_clock *clock) {
    ftime(&clock->last_time);
}

double hc_clock_step(hc_clock *clock) {
    struct timeb now;
    ftime(&now);
    double delta = (now.time - clock->last_time.time) + (now.millitm - clock->last_time.millitm)/1000.0;
    clock->last_time = now;
    return delta;
}

#endif
