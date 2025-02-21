#include <time.h>

clock_t clock(void)
{
    struct timespec ts;
    if (__clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts))
        return -1;
    return ts.tv_sec * CLOCKS_PER_SEC + ts.tv_nsec / 1000;
}
