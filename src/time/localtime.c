#include <time.h>

struct tm *localtime(const time_t *t)
{
    static struct tm buf;
    return localtime_r(t, &buf);
}
