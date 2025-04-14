#include <stdlib.h>
#include <pthread.h>

struct block {
    void (*fn[32])(void);
    struct block *next;
};

static struct block builtin;
static struct block *head = &builtin;
static int n;
static struct lock lock;

void __run_atexit_funcs(void)
{
    __lock(&lock);
    while (head) {
        if (n) {
            void (*f)(void) = head->fn[--n];
            __unlock(&lock);
            f();
            __lock(&lock);
        }
        else
        {
            head = head->next;
            n = 32;
        }
    }
    __unlock(&lock);
}

int atexit(void (*f)(void))
{
    __lock(&lock);
    if (n == 32) {
        struct block *p = malloc(sizeof *p);
        if (!p) {
            __unlock(&lock);
            return -1;
        }
        p->next = head;
        head = p;
        n = 0;
    }
    head->fn[n++] = f;
    __unlock(&lock);
    return 0;
}
