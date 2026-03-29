#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/socket.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

static void mkptr4(char *buf, const unsigned char *a)
{
    sprintf(buf, "%d.%d.%d.%d.in-addr.arpa", a[3], a[2], a[1], a[0]);
}

static void mkptr6(char *p, const unsigned char *a)
{
    static const char alphabet[16] = "0123456789abcdef";
    for (int i = 0; i < 16; i++)
    {
        *p++ = alphabet[a[15-i] & 0xf];
        *p++ = '.';
        *p++ = alphabet[a[15-i] >> 4];
        *p++ = '.';
    }
    memcpy(p, "ip6.arpa", 9);
}

struct procctx {
    char *node;
    size_t nodelen;
    int rv;
};

static int proc_dns(const unsigned char *rr, size_t rrlength,
        const unsigned char *packet, size_t packetlength, int type, void *c)
{
    struct procctx *ctx = c;
    if (type == DNS_RR_PTR)
    {
        ctx->rv = !__dn_expand(rr, packet, packetlength, ctx->node, ctx->nodelen);
        return 1;
    }
    return 0;
}

static int reverse_node_from_dns(int af, const unsigned char *a, char *node,
        size_t nodelen)
{
    char ptrname[16*4+sizeof "ip6.arpa"];
    if (af == AF_INET) mkptr4(ptrname, a);
    else mkptr6(ptrname, a);
    struct resolvconf conf;
    __get_resolvconf(&conf);

    unsigned char query[DNS_MAXQUERY];
    unsigned char result[DNS_MAXRESULT];
    size_t reslen;
    __dns_create_query(ptrname, DNS_RR_PTR, query, sizeof query);
    if (__dns_transact(&(unsigned char *){query}, &(size_t){sizeof query}, 1,
            &conf, &(unsigned char *){result}, &reslen))
        return EAI_SYSTEM;
    struct procctx procctx = {node, nodelen};
    __dns_process(result, reslen, proc_dns, &procctx);
    return procctx.rv;
}

static int reverse_node_from_hosts(int af, const unsigned char *a, int scope,
        char *node, size_t nodelen)
{
    char linebuf[LINE_MAX];
    FILE *f = fopen("/etc/hosts", "r");
    if (!f) return 0;
    int rv = 0;
    while (fgets(linebuf, sizeof linebuf, f)) {
        char *p = strchr(linebuf, '#');
        if (p) {
            *p++ = ' ';
            *p = 0;
        }
        for (p = linebuf; *p && !isspace((unsigned char)*p); p++);
        *p++ = 0;
        struct address addr;
        if (__lookup_ipliteral(linebuf, af, &addr) == 1
                && ((af == AF_INET && !memcmp(a, addr.addr + 12, 4))
                    || (af == AF_INET6 && !memcmp(a, addr.addr, 16)
                        && addr.scope_id == scope)))
        {
            /* skip leading whitespace */
            for (; isspace((unsigned char)*p); p++);
            char *q = p;
            /* find end of name */
            for (; *q && !isspace((unsigned char)*q); q++);
            if (q - p < nodelen) {
                memcpy(node, p, q - p);
                node[q - p] = 0;
                rv = 1;
            } else rv = EAI_OVERFLOW;
            break;
        }
    }
    fclose(f);
    return rv;
}

static int reverse_node(int af, const unsigned char *a, int scope, char *node,
        size_t nodelen)
{
    int rv = reverse_node_from_hosts(af, a, scope, node, nodelen);
    if (rv) return rv;
    return reverse_node_from_dns(af, a, node, nodelen);
}

static int reverse_service(int port, int is_dgram, char *svc, size_t svclen)
{
    if (port >= 65536u) return 0; /* not sure how to even trigger this. */
    FILE *f = fopen("/etc/services", "r");
    if (!f) return 0;
    char search[5+4+1];
    int searchlen = snprintf(search, sizeof search, "%d/%s", port, is_dgram? "udp" : "tcp");
    char linebuf[LINE_MAX];

    int rv = 0;
    while (fgets(linebuf, sizeof linebuf, f))
    {
        char *p = strchr(linebuf, '#');
        if (p) {
            *p++ = ' ';
            *p = 0;
        }
        p = strstr(linebuf, search);
        if (p && p > linebuf
                && isspace((unsigned char)p[-1])
                && isspace((unsigned char)p[searchlen])) {
            for (p = linebuf; *p && !isspace((unsigned char)*p); p++);
            if (p - linebuf < svclen) {
                memcpy(svc, linebuf, p - linebuf);
                svc[p - linebuf] = 0;
                rv = 1;
            } else
                rv = EAI_OVERFLOW;
            break;
        }
    }
    fclose(f);
    return rv;
}

int getnameinfo(const struct sockaddr *restrict sa, socklen_t salen,
        char *restrict node, socklen_t nodelen,
        char *restrict svc, socklen_t svclen, int flags)
{
    const unsigned char *a;
    static const char unspec[16] = "";
    int port;
    int af;
    int scope = 0;
    if (salen < sizeof (sa_family_t)
            || (sa->sa_family != AF_INET && sa->sa_family != AF_INET6)
            || (sa->sa_family == AF_INET && salen < sizeof (struct sockaddr_in))
            || (sa->sa_family == AF_INET6 && salen < sizeof (struct sockaddr_in6)))
        return EAI_FAMILY;

    if (flags > 63 || (flags & (NI_NUMERICHOST | NI_NAMEREQD)) == (NI_NUMERICHOST | NI_NAMEREQD))
        return EAI_BADFLAGS;

    af = sa->sa_family;
    if (af == AF_INET) {
        port = ntohs(((struct sockaddr_in *)sa)->sin_port);
        a = (const unsigned char *)sa + offsetof(struct sockaddr_in, sin_addr);
    } else {
        port = ntohs(((struct sockaddr_in6 *)sa)->sin6_port);
        a = (const unsigned char *)sa + offsetof(struct sockaddr_in6, sin6_addr);
        scope = ((struct sockaddr_in6 *)sa)->sin6_scope_id;
        if (!memcmp(a, __v4mapped_prefix, 12)) {
            a += 12;
            af = AF_INET;
            scope = 0;
        }
    }

    int rv = 0;
    if (svc && svclen) {
        if (!(flags & NI_NUMERICSERV)) {
            rv = reverse_service(port, flags & NI_DGRAM, svc, svclen);
            if (rv < 0) return rv;
        }
        if (rv == 0) {
            int len = snprintf(svc, svclen, "%d", port);
            if (len >= svclen) return EAI_OVERFLOW;
        }
    }

    if (node && nodelen) {
        rv = 0;
        if (!(flags & NI_NUMERICHOST)) {
            if (af != AF_INET6 || memcmp(a, unspec, 16))
                rv = reverse_node(af, a, scope, node, nodelen);
            if (rv < 0) return rv;
            if (rv == 0 && (flags & NI_NAMEREQD))
                return EAI_NONAME;
        }
        if (rv == 0) {
            char addrbuf[INET6_ADDRSTRLEN + 2 + IF_NAMESIZE];
            int addrlen = 0;
            const struct sockaddr_in6 *s6 = (void *)sa;
            inet_ntop(af, a, addrbuf, sizeof addrbuf);
            addrlen = strlen(addrbuf);
            if (af == AF_INET6
                    && (IN6_IS_ADDR_LINKLOCAL(&s6->sin6_addr)
                        || IN6_IS_ADDR_MC_LINKLOCAL(&s6->sin6_addr))) {
                addrbuf[addrlen++] = '%';
                if (flags & NI_NUMERICSCOPE) {
                    addrlen += snprintf(addrbuf + addrlen, sizeof addrbuf - addrlen, "%d", scope);
                } else {
                    if (!if_indextoname(scope, addrbuf + addrlen))
                        return EAI_SYSTEM;
                    addrlen += strlen(addrbuf + addrlen);
                }
            }
            if (addrlen >= nodelen)
                return EAI_OVERFLOW;
            memcpy(node, addrbuf, addrlen + 1);
        }
    }
    return 0;
}
