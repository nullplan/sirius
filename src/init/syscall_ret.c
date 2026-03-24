#include "syscall.h"

hidden long __set_errno(long v)
{
    errno = v;
    return -1;
}

hidden long __syscall_ret(long rv)
{
    if (rv > -4096UL)
        rv = __set_errno(-rv);
    return rv;
}
