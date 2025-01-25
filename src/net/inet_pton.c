#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

struct numdata {
    size_t len;
    int err;
    int num;
};

static struct numdata getdecbyte(const char *str)
{
    size_t i = 0;
    int rv = 0;
    if (!isdigit(*str)) return (struct numdata){0, 1, 0};
    while (isdigit(str[i]) && rv < 255) {
        rv = 10 * rv + (str[i++]) - '0';
    }
    if (rv >= 256 || isdigit(*str)) return (struct numdata){0, 1, 0};
    return (struct numdata){i, 0, rv};
}

static struct numdata gethexhalf(const char *str)
{
    size_t i = 0;
    int rv = 0;
    if (!isxdigit(*str)) return (struct numdata){0, 1, 0};
    while (isxdigit(str[i]) && rv < 65535) {
        int d = isdigit(str[i])? str[i] - '0' : (str[i]|32) - 'a' + 10;
        rv = (rv << 4) | d;
    }
    if (rv >= 65536 || isxdigit(str[i])) return (struct numdata){0, 1, 0};
    return (struct numdata){i, 0, rv};
}

static int do_ipv4(const char *restrict str, unsigned char *d)
{
    struct numdata nd;
    for (size_t i = 0; i < 4; i++) {
        nd = getdecbyte(str);
        if (nd.err || (i < 3 && str[nd.len] != '.')
                || (i == 3 && str[nd.len]))
            return 0;
        d[i] = nd.num;
        str += nd.len + 1;
    }
    return 1;
}
int inet_pton(int af, const char *restrict str, void *restrict dst)
{
    struct numdata nd;
    unsigned char *d = dst;
    if (af == AF_INET) {
        return do_ipv4(str, d);
    } else if (af == AF_INET6) {
        unsigned short part[8];
        size_t i;
        int brk = -1;
        int tryv4 = 0;
        if (str[0] == ':' && str[1] == ':') str++;
        else if (str[0] == ':') return 0;
        for (i = 0; i < 8;) {
            if (*str == ':') {
                if (brk != -1) return 0;
                brk = i;
                str++;
            }
            nd = gethexhalf(str);
            if (nd.err) return 0;
            if (str[nd.len] == '.') {
                tryv4 = 1;
                break;
            }
            part[i++] = nd.num;
            if (str[nd.len] == ':')
                str += nd.len + 1;
            else if (!str[nd.len])
                break;
            else
                return 0;
        }

        if (tryv4) {
            unsigned char v4part[4];
            if (!do_ipv4(str, v4part)) return 0;
            part[i] = v4part[0] << 8 | v4part[1];
            part[i+1] = v4part[2] << 8 | v4part[3];
            i += 2;
        }
        if (brk == -1 && i != 8) return 0;
        if (brk != -1) {
            if (i == 8) return 0;
            memmove(part + 8 - (i - brk), part + brk, (i - brk) * sizeof (short));
            memset(part + brk, 0, (8 - i) * sizeof (short));
        }
        for (i = 0; i < 8; i++) {
            d[2*i] = part[i] >> 8;
            d[2*i + 1] = part[i];
        }
        return 1;
    } else {
        errno = EAFNOSUPPORT;
        return -1;
    }
}
