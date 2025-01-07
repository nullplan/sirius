#include <time.h>

struct tm *gmtime(const time_t *t)
{
    static struct tm buf;
    return gmtime_r(t, &buf);
}
