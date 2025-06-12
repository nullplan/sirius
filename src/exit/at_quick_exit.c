#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

struct block {
    void (*fn[32])(void);
    struct block *next;
};

static struct block builtin;
static struct block *head = &builtin;
static int n;
static struct lock lock;

void __run_quick_exit_funcs(void)
{
    sigset_t ss;
    __block_usr_sigs(&ss);
    __lock(&lock);
    while (head) {
        if (n) {
            void (*f)(void) = head->fn[--n];
            __unlock(&lock);
            __restore_sigs(&ss);
            f();
            __block_usr_sigs(&ss);
            __lock(&lock);
        }
        else
        {
            head = head->next;
            n = 32;
        }
    }
    __unlock(&lock);
    __restore_sigs(&ss);
}

int at_quick_exit(void (*f)(void))
{
    sigset_t ss;
    __block_usr_sigs(&ss);
    __lock(&lock);
    if (n == 32) {
        struct block *p = __libc_malloc(sizeof *p);
        if (!p) {
            __unlock(&lock);
            __restore_sigs(&ss);
            return -1;
        }
        p->next = head;
        head = p;
        n = 0;
    }
    head->fn[n++] = f;
    __restore_sigs(&ss);
    __unlock(&lock);
    return 0;
}
