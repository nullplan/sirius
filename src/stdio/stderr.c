#include <stdio.h>
static unsigned char buf[UNGET+BUFSIZ];
static FILE f = {
    .fd = 2,
    .flags = F_NORD | F_PERM,
    .buf = buf + UNGET,
    .buf_size = 0,
    .lbf = -1,
    .write = __stdio_write,
    .seek = __stdio_seek,
    .close = __stdio_close,
    .lock = -1,
};
hidden FILE *const __stderr_used = &f;
weak_alias(stderr, __stderr_used);
