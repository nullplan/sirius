#include <pthread.h>
#include <sched.h>
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *ds)
{
    *ds = attr->__flags & PTHREAD_CREATE_DETACHED;
    return 0;
}

int pthread_attr_getinheritsched(const pthread_attr_t *restrict attr, int *restrict is)
{
    *is = attr->__flags & PTHREAD_EXPLICIT_SCHED;
    return 0;
}

int pthread_attr_getschedparam(const pthread_attr_t *restrict attr, struct sched_param *restrict sp)
{
    sp->sched_priority = attr->__prio;
    return 0;
}

int pthread_attr_getschedpolicy(const pthread_attr_t *restrict attr, int *restrict pol)
{
    *pol = attr->__pol;
    return 0;
}

int pthread_attr_getscope(const pthread_attr_t *restrict attr, int *restrict sc)
{
    *sc = PTHREAD_SCOPE_SYSTEM;
    return 0;
}

int pthread_attr_getstack(const pthread_attr_t *restrict attr, void **restrict stack, size_t *restrict ss)
{
    *stack = attr->__addr;
    *ss = attr->__ss;
    return 0;
}

int pthread_attr_getstacksize(const pthread_attr_t *restrict attr, size_t *restrict ss)
{
    *ss = attr->__ss;
    return 0;
}

int pthread_attr_getguardsize(const pthread_attr_t *restrict attr, size_t *restrict gs)
{
    *gs = attr->__gs;
    return 0;
}
