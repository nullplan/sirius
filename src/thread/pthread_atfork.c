#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

static struct lock lock;
struct block {
    void (*fn[32][3])(void);
    struct block *next, *prev;
};

static size_t nptr;
static struct block builtin, *head = &builtin, *tail = &builtin;

static void sentinel(void) {}

hidden void __atfork_run(int x)
{
    __lock(&lock);
    if (x < 0) {
        size_t n = nptr;
        for (const struct block *p = tail; p; p = p->prev, n = 32)
            for (size_t i = n; i-- > 0;)
                p->fn[i][0]();
    } else {
        for (const struct block *p = head; p; p = p->next) {
            size_t n = p == tail? nptr : 32;
            for (size_t i = 0; i < n; i++)
                p->fn[i][x+1]();
        }
    }
    __unlock(&lock);
}


int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    if (!prepare && !parent && !child) return 0;
    if (!prepare) prepare = sentinel;
    if (!parent) parent = sentinel;
    if (!child) child = sentinel;

    __lock(&lock);
    if (nptr == 32)
    {
        struct block *next = __libc_malloc(sizeof (struct block));
        if (!next) {
            __unlock(&lock);
            return ENOMEM;
        }
        nptr = 0;
        tail->next = next;
        next->prev = tail;
        next->next = 0;
        tail = next;
    }
    tail->fn[nptr][0] = prepare;
    tail->fn[nptr][1] = parent;
    tail->fn[nptr][2] = child;
    nptr++;
    __unlock(&lock);
    return 0;
}
