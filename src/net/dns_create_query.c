#include <netdb.h>
#include <time.h>
#include <string.h>
#include <assert.h>

hidden int __dns_create_query(const char *name, int qtype, unsigned char *buf, size_t len)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    int id = now.tv_sec ^ now.tv_nsec;
    /* size of query: header (12 bytes) + 1 question (strlen(name) + 5) */
    size_t namelen = strlen(name);
    assert(namelen - 1u < 255u);
    if (len < namelen + 17)
        return -1;

    memset(buf + 3, 0, 9); /* clear header */
    buf[0] = id;        /* put low byte first, since it is the faster changing byte; reduces chance of collision with other queries. */
    buf[1] = id >> 8;
    buf[2] = 1; /* recursion desired */
    buf[5] = 1; /* qdcount = 1, all others = 0 */
    unsigned char *out = buf + 12;
    while (*name) {
        size_t sublen = __stridx(name, '.');
        assert(sublen - 1 < 63u);
        *out = sublen;
        memcpy(out + 1, name, sublen);
        out += sublen + 1;
        name += sublen + !!name[sublen];
    }
    *out++ = 0;
    out[0] = qtype >> 8;
    out[1] = qtype;
    out[2] = 0;
    out[3] = 1;
    return namelen + 17;
}
