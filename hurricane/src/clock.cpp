#include <hurricane/clock.h>
#include <stdlib.h>

#ifdef _WIN32
    int gettimeofday(struct timeval *tp, struct timezone *tzp) {
        // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
        // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
        // until 00:00:00 January 1, 1970

        static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

        SYSTEMTIME system_time;
        FILETIME file_time;
        uint64_t time;

        GetSystemTime(&system_time);
        SystemTimeToFileTime(&system_time, &file_time);
        time = ((uint64_t)file_time.dwLowDateTime);
        time += ((uint64_t)file_time.dwHighDateTime) << 32;

        tp->tv_sec = (long)((time - EPOCH) / 10000000L);
        tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
        return 0;
    }
#endif

void hc_clock_new(hc_clock *clock) {
    //gettimeofday(&clock->last_time, NULL);
}

fixed hc_clock_step(hc_clock *clock) {
    //struct timeval now;
    //gettimeofday(&now, NULL);
    //fixed delta = (now.tv_sec - clock->last_time.tv_sec) + (now.tv_usec - clock->last_time.tv_usec) / 1000000.0;
    //clock->last_time = now;
    return 0.1;
}
