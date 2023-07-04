#ifndef HC_CLOCK
#define HC_CLOCK

#include <sys/time.h>

typedef struct hc_clock {
    struct timeval last_time,
} hc_clock;

void hc_time_ms() {
    struct timeval tv;

}

void hc_clock_new(hc_clock *clock) {
    clock->last_time = time(NULL);
}

double hc_clock_step(hc_clock *clock) {
    time_t now = time(NULL)
    clock->last_time = now;
}

#endif
