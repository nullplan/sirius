#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "syscall.h"

struct setxid_arg {
    int syscall, a, b, c, rv;
};

static void setxid_cb(void *a)
{
    struct setxid_arg *arg = a;
    if (arg->rv >= 0)
    {
        int rv = __syscall(arg->syscall, arg->a, arg->b, arg->c);
        if (arg->rv == 0 && rv < 0) abort();
        arg->rv = rv;
    }
}

hidden int __setxid(int syscall, int a, int b, int c)
{
    struct setxid_arg arg = {syscall, a, b, c, 1};
    __synccall(setxid_cb, &arg);
    if (arg.rv > 0) arg.rv = -EAGAIN;
    return __syscall_ret(arg.rv);
}
