#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

static const unsigned char v4mapped_prefix[12] = {[10]=0xff, 0xff};
static int name_from_null(const char *name, int flags, int family, struct address addr[static MAXADDR])
{
    if (name) return 0;
    int cnt = 0;
    if (flags & AI_PASSIVE) {
        if (family != AF_INET6) {
            addr[cnt].af = AF_INET;
            memcpy(addr[cnt].addr, v4mapped_prefix, 12);
            memset(addr[cnt].addr + 12, 0, 4);
            cnt++;
        }
        if (family != AF_INET) {
            addr[cnt].af = AF_INET6;
            memset(addr[cnt].addr, 0, 16);
            addr[cnt].scope_id = 0;
            cnt++;
        }
    } else {
        if (family != AF_INET6) {
            addr[cnt].af = AF_INET;
            memcpy(addr[cnt].addr, v4mapped_prefix, 12);
            memcpy(addr[cnt].addr + 12, "\x7f\0\0\1", 4);
            cnt++;
        }
        if (family != AF_INET) {
            addr[cnt].af = AF_INET6;
            memset(addr[cnt].addr, 0, 15);
            addr[cnt].addr[15] = 1;
            addr[cnt].scope_id = 0;
            cnt++;
        }
    }
    return cnt;
}

static int name_from_literal(const char *name, int family, struct address addr[static MAXADDR])
{
    return __lookup_ipliteral(name, family, addr);
}

static int name_from_hosts(const char *name, int family, struct address addr[static MAXADDR], char *cname)
{
    FILE *f = fopen("/etc/hosts", "r");
    if (!f) return 0;
    char buf[LINE_MAX];

    int cnt = 0;
    size_t len = strlen(name);
    while (fgets(buf, sizeof buf, f)) {
        char *p = strchr(buf, '#');
        if (p) {
            *p++ = ' ';
            *p = 0;
        }

        p = strstr(buf + 1, name);
        if (p && isspace(p[-1]) && isspace(p[len])) {
            for (p = buf; !isspace(*p); p++);
            *p++ = 0;
            if (__lookup_ipliteral(buf, family, addr + cnt)) {
                if (!cnt) {
                    for (; isspace(*p); p++);
                    for (len = 0; len < 255 && !isspace(p[len]); len++)
                        cname[len] = p[len];
                    cname[len] = 0;
                }
                cnt++;
            }
        }
    }
    fclose(f);
    return cnt;
}

struct dns_ctx {
    struct address *addr;
    size_t naddr;
    char *cname;
    int reqtype;
};

static int process_cb(const unsigned char *data, size_t datalen, const unsigned char *res, size_t reslen, int rrtype, void *ctx)
{
    struct dns_ctx *c = ctx;
    if (rrtype == DNS_RR_CNAME
            && c->cname
            && !__dn_expand(data, res, reslen, c->cname, 256)) {
        c->cname = 0;
    }
    if (rrtype != c->reqtype || !c->naddr) return 0;
    if (rrtype == DNS_RR_A) {
        if (datalen != 4) return -1;
        c->addr->af = AF_INET;
        memcpy(c->addr->addr, v4mapped_prefix, 12);
        memcpy(c->addr->addr + 12, data, 4);
    } else {
        if (datalen != 16) return -1;
        c->addr->af = AF_INET6;
        c->addr->scope_id = 0;
        memcpy(c->addr->addr, data, 16);
    }
    c->addr++;
    c->naddr--;
    return 0;
}

static int name_from_fqdn(const char *name, int family, struct address addr[static MAXADDR], char *cname, const struct resolvconf *conf)
{
    unsigned char querybuf[2][DNS_MAXQUERY];
    unsigned char *queryptr[2] = {querybuf[0], querybuf[1]};
    unsigned char resultbuf[2][DNS_MAXRESULT];
    unsigned char *resultptr[2] = {resultbuf[0], resultbuf[1]};
    size_t resultlen[2] = {0};
    size_t querylen[2] = {0};
    int qtype[2];
    int nquery = 0;
    struct dns_ctx ctx = {
        .addr = addr,
        .naddr = MAXADDR,
        .cname = cname,
    };
    if (family != AF_INET6) {
        qtype[nquery] = DNS_RR_A;
        querylen[nquery] = __dns_create_query(name, DNS_RR_A, querybuf[nquery], sizeof querybuf[0]);
        nquery++;
    }

    if (family != AF_INET) {
        qtype[nquery] = DNS_RR_AAAA;
        querylen[nquery] = __dns_create_query(name, DNS_RR_AAAA, querybuf[nquery], sizeof querybuf[0]);
        nquery++;
    }

    if (nquery == 2 && querybuf[0][0] == querybuf[1][0])
        querybuf[1][0]++;

    if (__dns_transact(queryptr, querylen, nquery, conf, resultptr, resultlen)) return 0;

    for (int i = 0; i < nquery; i++) {
        if (resultlen[i] - 1 < DNS_MAXRESULT - 1) {
            ctx.reqtype = qtype[i];
            __dns_process(resultbuf[i], resultlen[i], process_cb, &ctx);
        }
    }
    return MAXADDR - ctx.naddr;
}

static int is_dns_name(const char *name)
{
    if (*name == '.') return 0;
    size_t lastdot = 0;
    size_t nl = 0;
    for (; nl < 256 && name[nl]; nl++) {
        if (name[nl] == '.') {
            if (nl - lastdot - 1 >= 63ul) return 0;
            lastdot = nl;
        }
    }
    if (name[nl]) return 0;
    return 1;
}

static int count_dots(const char *name)
{
    int rv = 0;
    while (*name) {
        if (*name++ == '.') rv++;
    }
    return rv;
}

static int name_from_dns(const char *name, int family, struct address addr[static MAXADDR], char *cname)
{
    if (!is_dns_name(name)) return 0;
    struct resolvconf conf;
    __get_resolvconf(&conf);
    size_t namelen = strlen(name);
    if (count_dots(name) >= conf.ndots && name[namelen - 1] != '.') {
        memcpy(cname, name, namelen);
        cname[namelen] = '.';
        const char *a = conf.search;
        const char *z;
        for (; *a; a = z) {
            while (isspace(*a)) a++;
            for (z = a; *z && !isspace(*z); z++);
            if (z - a < 255 - namelen) {
                memcpy(cname + namelen + 1, a, z - a);
                cname[namelen + 1 + (z - a)] = 0;
                int cnt = name_from_fqdn(cname, family, addr, cname, &conf);
                if (cnt) return cnt;
            }
        }
    }
    cname[namelen] = 0;
    return name_from_fqdn(name, family, addr, cname, &conf);
}

/* AI_ADDRCONFIG is unfeasible to implement exactly. I'd have to query all the addresses.
 * So I am approximating it by attempting to create a socket of the family and connect it
 * to the loopback address.
 */
static int is_af_usable(int af)
{
    int sk = socket(af, SOCK_DGRAM, 0);
    if (sk == -1) return 0;
    static const struct sockaddr_in sin = {.sin_family = AF_INET, .sin_port = 65535,
        .sin_addr.s_addr = 
#ifdef __LITTLE_ENDIAN
            0x0100007f
#else
            0x7f000001
#endif
    };
    static const struct sockaddr_in6 sin6 = {.sin6_family = AF_INET6, .sin6_port = 65535, .sin6_addr.s6_addr[15] = 1};
    int rv = 0;
    if (!connect(sk, af == AF_INET ? (void *)&sin : (void *)&sin6, af == AF_INET ? sizeof sin : sizeof sin6))
        rv = 1;
    close(sk);
    return rv;
}

/* Implementing RFC 6724 destination address selection by assigning a 31-bit sortkey to each element, then running qsort. */
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
    {{[15] = 1}, 15, 0xff, 50, 0},
    {{[10] = 0xff, 0xff}, 11, 0xff, 35, 4},
    {{0x20, 0x01}, 3, 0xff, 5, 5},
    {{0x20, 0x02}, 1, 0xff, 30, 2},
    {{0xfe, 0xc0}, 1, 0xc0, 1, 11},
    {{0xfc}, 0, 0xfe, 3, 13},
    {{0}, 0, 0, 40, 1},
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
    if (!memcmp(a, v4mapped_prefix, 12)) {
        if ((a[12] == 169 && a[13] == 254) || a[12] == 127) return 2; /* IPv4 zeroconf and loopback is link-local. */
        return 14;  /* everything else is global. */
    }
    if (a[0] == 0xfe) {
        if ((a[1] & 0xc0) == 0x80) return 2; /* link-local */
        if ((a[1] & 0xc0) == 0xc0) return 5; /* site-local */
    }
    return 14; /* everything else is global. */
}

static int matchprefix(const unsigned char *a, const unsigned char *b)
{
    int i, j = 0;
    for (i = 0; i < 16 && a[i] == b[i]; i++);
    if (i < 16) {
        unsigned char diff = a[i] ^ b[i];
        for (; j < 7 && !(diff & (1 << (7-j))); j++);
    }
    return i * 8 + j;
}

static int addrcmp(const void *a, const void *b)
{
    return ((const struct address *)b)->sortkey - ((const struct address *)a)->sortkey;
}

static int has_ipv6_addresses(const struct address *addr, size_t n)
{
    for (size_t i = 0; i < n; i++)
        if (addr->af == AF_INET6)
            return 1;
    return 0;
}

hidden int __lookup_name(const char *name, int flags, int family, struct address addr[static MAXADDR], char *cname)
{
    if (name) {
        size_t namelen = strnlen(name, 255);
        if (namelen - 1 > 254) return EAI_NONAME;
        memcpy(cname, name, namelen + 1);
    }

    if (flags & AI_V4MAPPED) {
        if (family == AF_INET6) family = AF_UNSPEC;
        else flags &= ~AI_V4MAPPED;
    }

    if (flags & AI_ADDRCONFIG) {
        if (family != AF_INET6 && !is_af_usable(AF_INET)) {
            if (family == AF_UNSPEC) family = AF_INET6;
            else return EAI_FAMILY;
        }
        if (family != AF_INET && !is_af_usable(AF_INET6)) {
            if (family == AF_UNSPEC) family = AF_INET;
            else return EAI_FAMILY;
        }
    }

    int cnt = name_from_null(name, flags, family, addr);
    if (!cnt) cnt = name_from_literal(name, family, addr);
    if (!cnt && !(flags & AI_NUMERICHOST)) {
        cnt = name_from_hosts(name, family, addr, cname);
        if (!cnt) cnt = name_from_dns(name, family, addr, cname);
    }
    
    if (cnt <= 0) return cnt? cnt : EAI_NONAME;

    if (flags & AI_V4MAPPED) {
        if ((flags & AI_ALL) || !has_ipv6_addresses(addr, cnt)) {
            for (int i = 0; i < cnt; i++)
                addr[i].af = AF_INET6;
        } else {
            int dst = 0;
            for (int i = 0; i < cnt; i++)
                if (addr[i].af == AF_INET6) {
                    if (dst != i)
                        addr[dst] = addr[i];
                    dst++;
                }
            cnt = dst;
        }
    }

    if (cnt == 1 || !has_ipv6_addresses(addr, cnt)) return cnt;

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
                memcpy(&sin6.sin6_addr, addr[i].addr, 16);
                p = (struct sockaddr *)&sin6;
                slen = sizeof sin6;
            }
            if (!connect(sk, p, slen) && !getsockname(sk, p, &slen)) {
                if (addr[i].af == AF_INET) {
                    memcpy(sa, v4mapped_prefix, 12);
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
    return cnt;
}
