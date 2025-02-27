#include <semaphore.h>
#include <errno.h>

int sem_init(sem_t *s, int ps, unsigned val)
{
    if (val > SEM_VALUE_MAX) {
        errno = EINVAL;
        return -1;
    }
    s->__ctr = val;
    s->__waiters = 0;
    s->__pshared = ps;
    return 0;
}
