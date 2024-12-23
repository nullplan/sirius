#include <stdio.h>
static unsigned char buf[UNGET+BUFSIZ];
static FILE f = {
    .fd = 0,
    .flags = F_NOWR | F_PERM,
    .buf = buf + UNGET,
    .buf_size = BUFSIZ,
    .lbf = -1,
    .read = __stdio_read,
    .seek = __stdio_seek,
    .close = __stdio_close,
    .lock = -1,
};
FILE *const stdin = &f;
hidden FILE *const __stdin_used = &f;
