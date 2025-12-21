#include <hurricane/clock.h>
#include <stdlib.h>
#include <hurricane/fixed.h>

void hc_clock_new(hc_clock *clock) {
    //gettimeofday(&clock->last_time, NULL);
}

fixed hc_clock_step(hc_clock *clock) {
    //struct timeval now;
    //gettimeofday(&now, NULL);
    //fixed delta = (now.tv_sec - clock->last_time.tv_sec) + (now.tv_usec - clock->last_time.tv_usec) / 1000000.0;
    //clock->last_time = now;
    return tf(0.1);
}
