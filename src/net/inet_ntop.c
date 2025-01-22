#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

const char *inet_ntop(int af, const void *restrict src, char *restrict dst, socklen_t dstlen)
{
    char buf[INET6_ADDRSTRLEN];
    const unsigned char *s = src;
    int len;
    if (af == AF_INET) {
        len = snprintf(buf, sizeof buf, "%d.%d.%d.%d", s[0], s[1], s[2], s[3]);
    } else if (af == AF_INET6) {
        if (!memcmp(s, "\0\0\0\0\0\0\0\0\0\0\xff\xff", 12))
            len = snprintf(buf, sizeof buf, "::ffff:%d.%d.%d.%d", s[12], s[13], s[14], s[15]);
        else {
            len = snprintf(buf, sizeof buf, "%x:%x:%x:%x:%x:%x:%x:%x",
                    s[0] << 8 | s[1], s[2] << 8 | s[3], s[4] << 8 | s[5], s[6] << 8 | s[7],
                    s[8] << 8 | s[9], s[10] << 8 | s[11], s[12] << 8 | s[13], s[14] << 8 | s[15]);
            char *longest = 0;
            size_t longlen = 2;
            for (char *p = buf; *p; p++) {
                if (p == buf || *p == ':') {
                    size_t dellen = strspn(p, ":0");
                    if (dellen + (p == buf) > longlen + (longest == buf)) {
                        longest = p;
                        longlen = dellen;
                    }
                }
            }
            if (longest) {
                longest[0] = longest[1] = ':';
                memmove(longest + 2, longest + longlen, len + 1 - (longest + longlen - buf));
                len -= longlen - 2;
            }
        }
    } else {
        errno = EAFNOSUPPORT;
        return 0;
    }

    if (dstlen <= len) {
        errno = ENOSPC;
        return 0;
    }
    return memcpy(dst, buf, len + 1);
}
