#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include "cpu.h"
/* Implementing RFC 6724 destination address selection by assigning a 31-bit sortkey to each element, then running qsort. */
/* the sortkey:
 *  3         2         1
 * 10987654321098765432109876543210
 * 0USL000000ppppppssssPPPPPPPPiiii
 *
 * Where:
 *  U - usable bit
 *  S - matching scope bit
 *  L - matching label bit
 *  p - precedence
 *  s - inverted scope
 *  P - matching prefix bits
 *  i - inverted original index
 */
#define DAS_USABLE          0x40000000  /* Rule 1: Prefer usable addresses. */
#define DAS_MATCHSCOPE      0x20000000  /* Rule 2: Prefer matching scope. */
/* Rules 3 and 4 are infeasible to implement. */
#define DAS_MATCHLABEL      0x10000000  /* Rule 5: Prefer matching label. */
#define DAS_PRECSHIFT       16          /* Rule 6: Prefer higher precedence. */
/* Rule 7 infeasible to implement. */
#define DAS_SCOPESHIFT      12          /* Rule 8: prefer smaller scope. */
#define DAS_PREFIXSHIFT     4           /* Rule 9: prefer longest matching prefix. */
#define DAS_INDEXSHIFT      0           /* Rule 10: keep original order. */
/* RFC 6724 §2.1 table, but sorted by prefix length descending. */
/* and with 6bone and ipv4compat deleted, since they are deprecated/gone. */
struct rule {
    unsigned char addr[16];
    unsigned char len, mask, prec, label;
};
static const struct rule rules[] = {
    {{[15] = 1},            15, 0xff, 50,  0}, /* IPv6 loopback address */
    {{[10] = 0xff, 0xff},   11, 0xff, 35,  4}, /* IPv4 mapped addresses */
    {{0x20, 0x01},           3, 0xff,  5,  5}, /* IETF protocol assignments */
    {{0x20, 0x02},           1, 0xff, 30,  2}, /* TEREDO */
    {{0xfe, 0xc0},           1, 0xc0,  1, 11}, /* site-local addresses (now deprecated) */
    {{0xfc},                 0, 0xfe,  3, 13}, /* unique local unicast */
    {{0}, 0, 0, 40, 1},                        /* catch-all */
};

static const struct rule *ruleof(const unsigned char *a)
{
    for (const struct rule *r = rules;; r++)
        if (!memcmp(a, r->addr, r->len) && (a[r->len] & r->mask) == r->addr[r->len])
            return r;
}

static int scopeof(const unsigned char *a)
{
    if (a[0] == 0xff) return a[1] & 0xf;    /* multicast */
    if (!memcmp(a, __v4mapped_prefix, 12)) {
        if ((a[12] == 169 && a[13] == 254) || a[12] == 127) return 2; /* IPv4 zeroconf and loopback is link-local. */
        return 14;  /* everything else is global. */
    }
    if (a[0] == 0xfe) {
        if ((a[1] & 0xc0) == 0x80) return 2; /* link-local */
        if ((a[1] & 0xc0) == 0xc0) return 5; /* site-local */
    }
    static const unsigned char loopback[16] = {[15] = 1};
    if (!memcmp(a, loopback, 16)) return 2;
    return 14; /* everything else is global. */
}

static int matchprefix(const unsigned char *a, const unsigned char *b)
{
    int i, j = 0;
    for (i = 0; i < 16 && a[i] == b[i]; i++);
    if (i < 16) {
        unsigned char diff = a[i] ^ b[i];
        j = a_clz(diff) - 8 * (sizeof (size_t) - 1);
    }
    return i * 8 + j;
}

static int addrcmp(const void *a, const void *b)
{
    return ((const struct address *)b)->sortkey - ((const struct address *)a)->sortkey;
}

hidden void __sort_addresses(struct address *addr, int cnt)
{
    struct sockaddr_in sin = {.sin_family = AF_INET};
    struct sockaddr_in6 sin6 = {.sin6_family = AF_INET6};
    for (int i = 0; i < cnt; i++)
    {
        int k = 0;
        const struct rule *dstrule = ruleof(addr[i].addr);
        int dstscope = scopeof(addr[i].addr);
        int sk = socket(addr[i].af, SOCK_DGRAM, 0);
        if (sk != -1)
        {
            unsigned char sa[16];
            struct sockaddr *p;
            socklen_t slen;
            if (addr[i].af == AF_INET) {
                sin.sin_port = 65535;
                memcpy(&sin.sin_addr, addr[i].addr + 12, 4);
                p = (struct sockaddr *)&sin;
                slen = sizeof sin;
            } else {
                sin6.sin6_port = 65535;
                sin6.sin6_scope_id = addr[i].scope_id;
                memcpy(&sin6.sin6_addr, addr[i].addr, 16);
                p = (struct sockaddr *)&sin6;
                slen = sizeof sin6;
            }
            if (!connect(sk, p, slen) && !getsockname(sk, p, &slen)) {
                if (addr[i].af == AF_INET) {
                    memcpy(sa, __v4mapped_prefix, 12);
                    memcpy(sa + 12, &sin.sin_addr, 4);
                } else {
                    memcpy(sa, &sin6.sin6_addr, 16);
                }
                const struct rule *srcrule = ruleof(sa);
                k |= DAS_USABLE;
                if (dstscope == scopeof(sa)) k |= DAS_MATCHSCOPE;
                if (dstrule->label == srcrule->label) k |= DAS_MATCHLABEL;
                k |= matchprefix(sa, addr[i].addr) << DAS_PREFIXSHIFT;
            }
            close(sk);
        }
        k |= dstrule->prec << DAS_PRECSHIFT;
        k |= (15-dstscope) << DAS_SCOPESHIFT;
        k |= (MAXADDR-1-i) << DAS_INDEXSHIFT;
        addr[i].sortkey = k;
    }
    qsort(addr, cnt, sizeof (struct address), addrcmp);
}
