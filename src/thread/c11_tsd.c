#include <threads.h>
#include <pthread.h>

int tss_create(tss_t *key, tss_dtor_t dtor)
{
    return __pthread_key_create((pthread_key_t *)key, dtor) == 0? thrd_success : thrd_error;
}

void tss_delete(tss_t key)
{
    pthread_key_t k = {key.__k};
    __pthread_key_delete(k);
}

void *tss_get(tss_t key)
{
    return __pthread_self()->tsd[key.__k];
}

int tss_set(tss_t key, void *val)
{
    __pthread_self()->tsd[key.__k] = val;
    return 0;
}
