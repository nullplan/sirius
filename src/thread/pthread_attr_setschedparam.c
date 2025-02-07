#include <pthread.h>
#include <sched.h>

int pthread_attr_setschedparam(pthread_attr_t *restrict attr, const struct sched_param *restrict sp)
{
    attr->__prio = sp->sched_priority;
    return 0;
}
