#ifndef DIRENT_H
#define DIRENT_H

#include "../../include/dirent.h"
#include <pthread.h>
#include <assert.h>
#include <stddef.h>
#include <sys/types.h>

#define DIRBUFSIZ   4000
struct __dirstream {
    int fd;
    struct lock lock;
    unsigned char *pos, *end;
    off_t tell;
    unsigned char buf[];
};

static_assert(offsetof(struct __dirstream, buf) % _Alignof(off_t) == 0, "");
#endif
