#include <time.h>

__deprecated char *ctime(const time_t *t)
{
    return asctime(localtime(t));
}
