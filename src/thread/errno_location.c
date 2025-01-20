#include <errno.h>

int *__errno_location(void)
{
    return &__pthread_self()->errno_val;
}
