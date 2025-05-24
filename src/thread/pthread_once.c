#include <pthread.h>
#include <stdlib.h>

int pthread_once(pthread_once_t *flg, void (*fct)(void))
{
    call_once((once_flag *)flg, fct);
    return 0;
}
