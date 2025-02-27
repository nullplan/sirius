#include <semaphore.h>
#include <time.h>

int sem_timedwait(sem_t *restrict sem, const struct timespec *restrict to)
{
    return sem_clockwait(sem, CLOCK_REALTIME, to);
}
